#include "Telemetry.h"

Telemetry::Telemetry() {
  this->solarPanel = false;
  this->gear = false;
  this->docked = false;
  this->lights = false;
  this->sas = false;
  this->rcs = false;
  this->brake = false;
  this->engine = false;
  this->antenna = false;

  this->twr = 0;
  this->pitch = 0;

  this->altitude = 0.0;
  this->periapsis = 0.0;
  this->apoapsis =  0.0;
  this->verticalVelocity =  0.0;
  this->horizontalVelocity =  0.0;
}