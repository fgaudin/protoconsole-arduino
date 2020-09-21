#include "Controller.h"

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

const unsigned long refreshPeriod = 100000; // 100 ms
const int inputInterruptPin = 2;
const int inputClockPin = 3;
const int inputLoadPin = 4;
const int inputSerialIn = 5;
const int pinBarData = 8;
const int pinBarClock = 9;
const int pinBarLoad = 10;
const int pinLedData = 11;
const int pinLedClock = 12;
const int pinLedLoad = 13;

volatile int incoming;

Controller::Controller() {
  this->lastUpdate = micros();
  this->handlers[0] = nullptr;
  this->handlers[1] = &Controller::handle_hello;
  this->handlers[2] = nullptr;
  this->handlers[8] = &Controller::handle_flags1;
  this->handlers[9] = &Controller::handle_twr;
  this->handlers[10] = &Controller::handle_pitch;
  this->handlers[11] = &Controller::handleStageFuel;
  this->handlers[12] = &Controller::handleStageOx;
  this->handlers[13] = &Controller::handleStageMonoprop;
  this->handlers[14] = &Controller::handleStageElec;
  this->handlers[15] = &Controller::handleStageXenon;
  this->handlers[16] = &Controller::handleStageO2;
  this->handlers[17] = &Controller::handleStageH2O;
  this->handlers[18] = &Controller::handleStageFood;
  this->handlers[19] = &Controller::handleStageCO2;
  this->handlers[20] = &Controller::handleStageWaste;
  this->handlers[21] = &Controller::handle_flags2;
  this->handlers[32] = &Controller::handle_periapsis;
  this->handlers[33] = &Controller::handle_apoapsis;
  this->handlers[34] = &Controller::handle_altitude;
  this->handlers[35] = &Controller::handle_vertical_speed;
  this->handlers[36] = &Controller::handle_horizontal_speed;

  this->connected = false;
  this->button_states = 0;
  incoming = 0;
}

void readButtons(){
  digitalWrite(inputLoadPin, LOW);
  delayMicroseconds(1);
  digitalWrite(inputLoadPin, HIGH);
  delayMicroseconds(1);

  digitalWrite(inputClockPin, LOW);
  for (int i=0;i<16; i++) {
    bitWrite(incoming, i, digitalRead(inputSerialIn));
    digitalWrite(inputClockPin, HIGH);
    delayMicroseconds(1);
    digitalWrite(inputClockPin, LOW);
  }
}

void Controller::init()
{
  pinMode(inputClockPin, OUTPUT);
  pinMode(inputLoadPin, OUTPUT);
  pinMode(inputSerialIn, INPUT);
  pinMode(inputInterruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(inputInterruptPin), readButtons, CHANGE);

  this->leds.init(pinLedData, pinLedClock, pinLedLoad, &this->telemetry);
  this->display.init(&this->telemetry);
  strcpy(this->display.debug_str, "controller ready");
  this->display.setMode(debug);

  this->bars.init(pinBarData, pinBarClock, pinBarLoad, &this->telemetry);
}

void Controller::checkInputs()
{
  if (bitRead(incoming, 2)) {
    strcpy(this->display.debug_str, "Undock");
  }
  if (bitRead(incoming, 3)) {
    strcpy(this->display.debug_str, "Lights");
  }
  if (bitRead(incoming, 4)) {
    strcpy(this->display.debug_str, "RCS");
  }
  if (bitRead(incoming, 5)) {
    strcpy(this->display.debug_str, "SAS");
  }
  if (bitRead(incoming, 6)) {
    strcpy(this->display.debug_str, "Stage");
  }
  if (incoming ^ this->button_states) {
    this->button_states = incoming;
    for (int i=0; i<16; i++) {
      if (incoming & (1 << (16 - i))) {
        this->display.debug_str[i] = '1';
      } else {
        this->display.debug_str[i] = '0';
      }
    }
    this->display.debug_str[16] = '\0';
    Serial.write(B11111111);
    Serial.write(B00000000);
    Serial.write((byte *) &incoming, 2);
  }
  readButtons();

  /*
  if(!digitalRead(pinAscentMode)) {
    this->display.setMode(ascent);
  }
  if(!digitalRead(pinDebugMode)) {
    this->display.setMode(debug);
  }
  if(!digitalRead(pinLifeSupportMode)) {
    this->bars.setMode(lifesupport);
  }
  if(!digitalRead(pinFuelMode)) {
    this->bars.setMode(fuel);
  }*/
}

void Controller::update()
{
  unsigned long now = micros();
  if (now - this->lastUpdate > refreshPeriod) {
    this->checkInputs();
    this->display.refresh();
    this->bars.refresh();
    this->leds.refresh();
    this->lastUpdate = now;
  }
}

void Controller::handle_command(byte command, byte* value)
{
  if (this->handlers[command] == nullptr) {
    //debug("unsupported cmd");
  } else {
    CALL_MEMBER_FN(*this, this->handlers[command]) (value);
  }
}

void Controller::handle_hello(byte* value)
{
  this->connected = true;
}

void Controller::handle_flags1(byte* value)
{
  this->telemetry.solarPanel = (int) * (byte *) value & B00000011;
  this->telemetry.gear = ((int) * (byte *) value & B00001100) >> 2;
  this->telemetry.antenna = ((int) * (byte *) value & B00110000) >> 4;
  this->telemetry.stage = bitRead(value[0], 6);
}

void Controller::handle_flags2(byte* value) {
  this->telemetry.rcs = bitRead(value[0], 0);
  this->telemetry.sas = bitRead(value[0], 1);
  this->telemetry.brake = bitRead(value[0], 2);
  this->telemetry.docked = bitRead(value[0], 3);  
  this->telemetry.lights = bitRead(value[0], 4);
  this->telemetry.dot05g = bitRead(value[0], 5);
  this->telemetry.contact = bitRead(value[0], 6);
  this->telemetry.master_alarm = bitRead(value[0], 7);
}

void Controller::handle_twr(byte* value)
{
  this->telemetry.twr = float(* (byte *) value) / 10;
}

void Controller::handleStageFuel(byte* value)
{
  this->telemetry.stageFuel =(int) * (byte *) value;
}

void Controller::handleStageOx(byte* value)
{
  this->telemetry.stageOx =(int) * (byte *) value;
}

void Controller::handleStageMonoprop(byte* value)
{
  this->telemetry.stageMonoprop =(int) * (byte *) value;
}

void Controller::handleStageElec(byte* value)
{
  this->telemetry.stageElec =(int) * (byte *) value;
}

void Controller::handleStageXenon(byte* value)
{
  this->telemetry.stageXenon =(int) * (byte *) value;
}

void Controller::handleStageO2(byte* value)
{
  this->telemetry.stageO2 =(int) * (byte *) value;
}

void Controller::handleStageH2O(byte* value)
{
  this->telemetry.stageH2O =(int) * (byte *) value;
}

void Controller::handleStageFood(byte* value)
{
  this->telemetry.stageFood =(int) * (byte *) value;
}

void Controller::handleStageCO2(byte* value)
{
  this->telemetry.stageCO2 =(int) * (byte *) value;
}

void Controller::handleStageWaste(byte* value)
{
  this->telemetry.stageWaste =(int) * (byte *) value;
}

void Controller::handle_pitch(byte* value)
{
  this->telemetry.pitch =(int) * (signed char *) value;
}

void Controller::handle_periapsis(byte* value)
{
  this->telemetry.periapsis = * (long *) value;
}

void Controller::handle_apoapsis(byte* value)
{
  this->telemetry.apoapsis = * (long *) value;
}

void Controller::handle_vertical_speed(byte* value)
{
  this->telemetry.verticalSpeed = * (long *) value;
}

void Controller::handle_horizontal_speed(byte* value)
{
  this->telemetry.horizontalSpeed = * (long *) value;
}

void Controller::handle_altitude(byte* value)
{
  this->telemetry.altitude = * (long *) value;
}
