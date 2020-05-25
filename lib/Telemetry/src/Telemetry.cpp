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

  this->twr = 0.0;
  this->pitch = 0;
  this->stageFuel = 6;
  this->stageOx = 3;
  this->stageMonoprop = 8;
  this->stageElec = 1;
  this->stageXenon = 5;

  this->altitude = 0.0;
  this->periapsis = 0.0;
  this->apoapsis =  0.0;
  this->verticalSpeed =  0.0;
  this->horizontalSpeed =  0.0;
}