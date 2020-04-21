#include "Controller.h"

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

Controller::Controller() {
  strcpy(this->debug_str, "");

  this->handlers[0] = nullptr;
  this->handlers[1] = &Controller::handle_hello;
  this->handlers[2] = nullptr;
  this->handlers[64] = &Controller::handle_flags1;
  this->handlers[192] = &Controller::handle_periapsis;

  this->connected = false;

  this->solar_panel = false;
  this->gear = false;
  this->docked = false;
  this->lights = false;
  this->sas = false;
  this->rcs = false;
  this->brake = false;
  this->engine = false;
  this->antenna = false;

  this->periapsis = false;
  this->apoapsis = false;
  this->surface_velocity = false;
  this->orbital_velocity = false;
}

void Controller::handle_command(byte command, byte* value)
{
  strcpy(this->debug_str, "coucou");
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
  this->solar_panel = bitRead(value[0], 0);
  this->gear = bitRead(value[0], 1);
  this->docked = bitRead(value[0], 2);
  this->lights = bitRead(value[0], 3);
  this->rcs = bitRead(value[0], 4);
  this->sas = bitRead(value[0], 5);
  this->brake = bitRead(value[0], 6);
  this->antenna = bitRead(value[0], 7);
}

void Controller::handle_periapsis(byte* value)
{
  this->periapsis = * (long *) value;
}
