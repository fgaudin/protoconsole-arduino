#include "Telemetry.h"

Telemetry::Telemetry() {
  this->solarPanel = 0;
  this->gear = 0;
  this->docked = false;
  this->lights = false;
  this->sas = false;
  this->rcs = false;
  this->brake = false;
  this->engine = false;
  this->antenna = false;

  this->twr = 0.0;
  this->q = 0.0;
  this->pitch = 0;
  this->stageFuel = 0;
  this->stageOx = 0;
  this->stageMonoprop = 0;
  this->stageElec = 0;
  this->stageXenon = 0;
  this->stageO2 = 0;
  this->stageH2O = 0;
  this->stageFood = 0;
  this->stageCO2 = 0;
  this->stageWaste = 0;

  this->altitude = 0.0;
  this->periapsis = 0.0;
  this->apoapsis =  0.0;
  this->verticalSpeed =  0.0;
  this->horizontalSpeed =  0.0;
}