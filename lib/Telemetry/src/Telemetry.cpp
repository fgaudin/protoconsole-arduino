#include "Telemetry.h"

Telemetry::Telemetry() {
  this->solar_panel = false;
  this->gear = false;
  this->docked = false;
  this->lights = false;
  this->sas = false;
  this->rcs = false;
  this->brake = false;
  this->engine = false;
  this->antenna = false;

  this->periapsis = 0.0;
  this->apoapsis =  0.0;
  this->surface_velocity =  0.0;
  this->orbital_velocity =  0.0;
}