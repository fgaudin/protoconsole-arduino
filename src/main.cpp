#include <Arduino.h>
#include <SoftwareSerial.h>
#include <LedDisplay.h>
#include <Telemetry.h>
#include <BarGraph.h>
#include <MainDisplay.h>
#include <SevenSegment.h>
#include <KerbalSimpit.h>
#include <Utils.h>

#define DEBUG 1
#define TEST 0

const int pinDebugRx = 6;
const int pinDebugTx = 7;

// leds
const int pinLedData = 11;
const int pinLedClock = 12;
const int pinLedLoad = 13;
// bars
const int pinBarData = 8;
const int pinBarClock = 9;
const int pinBarLoad = 10;

KerbalSimpit mySimpit(Serial);

Telemetry telemetry;
LedDisplay leds;
BarGraph bars;
MainDisplay display;
SevenSegment seg7;

void messageHandler(byte messageType, byte msg[], byte msgSize);

void setup()
{
  Serial.begin(115200);

  while (!mySimpit.init()) {
    delay(100);
  }
  mySimpit.printToKSP("Connected", PRINT_TO_SCREEN);

  mySimpit.inboundHandler(messageHandler);
  mySimpit.registerChannel(ALTITUDE_MESSAGE);
  mySimpit.registerChannel(ACTIONSTATUS_MESSAGE);
  mySimpit.registerChannel(LF_STAGE_MESSAGE);
  mySimpit.registerChannel(OX_STAGE_MESSAGE);
  mySimpit.registerChannel(XENON_GAS_STAGE_MESSAGE);
  mySimpit.registerChannel(MONO_MESSAGE);
  mySimpit.registerChannel(ELECTRIC_MESSAGE);
  mySimpit.registerChannel(FLIGHT_STATUS_MESSAGE);

  // leds
  leds.init(pinLedData, pinLedClock, pinLedLoad, &telemetry);
  bars.init(pinBarData, pinBarClock, pinBarLoad, &telemetry);

  // LCD
  display.init(&telemetry);
  strcpy(display.debug_str, "controller ready");
  display.setMode(ascent);
  display.refresh();

  // 7 segments
  seg7.init(&telemetry);

  // bars
  bars.init(pinBarData, pinBarClock, pinBarLoad, &telemetry);
  bars.setMode(fuel);
}

void loop() {
  mySimpit.update();
  leds.refresh();
  display.update();
  seg7.refresh();
  bars.refresh();
}

void messageHandler(byte messageType, byte msg[], byte msgSize) {
  switch(messageType) {
    case ALTITUDE_MESSAGE:
      if (msgSize == sizeof(altitudeMessage)) {
        altitudeMessage myAltitude = parseAltitude(msg);
        if (telemetry.altitude != myAltitude.sealevel) {
          telemetry.altitude = myAltitude.sealevel;
          mySimpit.printToKSP("refresh");
          display.refresh();
        }
      }
    break;
    case ACTIONSTATUS_MESSAGE:
      if (msgSize == sizeof(byte)) {
        telemetry.sas = (bool) (msg[0] & SAS_ACTION);
        if (telemetry.sas) {
          mySimpit.printToKSP("SAS ON");
        } else {
          mySimpit.printToKSP("SAS OFF");
        }
        telemetry.lights = (bool) (msg[0] & LIGHT_ACTION);
        telemetry.rcs = (bool) (msg[0] & RCS_ACTION);
        telemetry.gear = (bool) (msg[0] & GEAR_ACTION);
        telemetry.brake = (bool) (msg[0] & BRAKES_ACTION);
      } else {
        mySimpit.printToKSP("wrong message size", VERBOSE_ONLY);
      }
    break;
    case LF_STAGE_MESSAGE:
    case OX_STAGE_MESSAGE:
    case XENON_GAS_STAGE_MESSAGE:
    case MONO_MESSAGE:
    case ELECTRIC_MESSAGE:
      if (msgSize == sizeof(resourceMessage)) {
        resourceMessage resource = parseResource(msg);
        int value = (int) round(10 * resource.available / resource.total);
        if (messageType == LF_STAGE_MESSAGE) telemetry.stageFuel = value;
        if (messageType == OX_STAGE_MESSAGE) telemetry.stageOx = value;
        if (messageType == XENON_GAS_STAGE_MESSAGE) telemetry.stageXenon = value;
        if (messageType == MONO_MESSAGE) telemetry.stageMonoprop = value;
        if (messageType == ELECTRIC_MESSAGE) telemetry.stageElec = value;
      }
    break;
    case FLIGHT_STATUS_MESSAGE:
      if (msgSize == sizeof(flightStatusMessage)) {
        flightStatusMessage flight = parseFlightStatusMessage(msg);
        telemetry.contact = (flight.vesselSituation <= 4);  // Landed: 1, splashed: 2, pre-launch: 4, flying: 8
      }
    break;
  }
}
