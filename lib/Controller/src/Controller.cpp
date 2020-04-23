#include "Controller.h"

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

Controller::Controller() {
  this->handlers[0] = nullptr;
  this->handlers[1] = &Controller::handle_hello;
  this->handlers[2] = nullptr;
  this->handlers[64] = &Controller::handle_flags1;
  this->handlers[192] = &Controller::handle_periapsis;

  this->connected = false;
}

void Controller::init()
{
  this->display.init(&this->telemetry);
  strcpy(this->display.debug_str, "controller ready");
  this->display.setMode(ascent);
}

void Controller::update()
{
  this->display.refresh();
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
  this->telemetry.solar_panel = bitRead(value[0], 0);
  this->telemetry.gear = bitRead(value[0], 1);
  this->telemetry.docked = bitRead(value[0], 2);
  this->telemetry.lights = bitRead(value[0], 3);
  this->telemetry.rcs = bitRead(value[0], 4);
  this->telemetry.sas = bitRead(value[0], 5);
  this->telemetry.brake = bitRead(value[0], 6);
  this->telemetry.antenna = bitRead(value[0], 7);
}

void Controller::handle_periapsis(byte* value)
{
  this->telemetry.periapsis = * (long *) value;
}
