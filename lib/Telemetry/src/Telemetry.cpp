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
  this->stage = false;

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

  this->met = 0;
}

void Telemetry::update(char id, byte* data) {
  if (id == 'f') {  // flags
    this->sas = (bool) (data[0] & 1);
    this->rcs = (bool) (data[0] & (1 << 1));
    this->lights = (bool) (data[0] & (1 << 2));
    this->brake = (bool) (data[0] & (1 << 3));
    this->contact = (bool) (data[0] & (1 << 4));
    this->dot05g = (bool) (data[0] & (1 << 5));
    this->docked = (bool) (data[0] & (1 << 6));
    this->stage = (bool) (data[0] & (1 << 7));
  } else if (id == 'g') {  // 2 bits flags
    this->solarPanel = (int) data[0] & B11;
    this->gear = (int) (data[0] >> 2) & B11;
    this->antenna = (int) (data[0] >> 4) & B11;
  } else if (id == 'u') {  // fuel bytes
    this->stageFuel = (int) data[0];
    this->stageOx = (int) data[1];
    this->stageMonoprop = (int) data[2];
    this->stageElec = (int) data[3];
    this->stageXenon = (int) data[4];
  } else if (id == 'l') {  // life support bytes
    this->stageO2 = (int) data[0];
    this->stageH2O = (int) data[1];
    this->stageFood = (int) data[2];
    this->stageCO2 = (int) data[3];
    this->stageWaste = (int) data[4];
  } else if (id == 'm') {
    this->met = data[0] << 16 | data[1] << 8 | data[2];
  } else if (id == 'a') {
    strcpy(this->apoapsis, (char *) data);
  } else if (id == 'p') {
    strcpy(this->periapsis, (char *) data);
  } else if (id == 'v') {
    strcpy(this->verticalSpeed, (char *) data);
  } else if (id == 'h') {
    strcpy(this->horizontalSpeed, (char *) data);
  } else if (id == 'A') {
    strcpy(this->altitude, (char *) data);
  } else if (id == 'P') {
    strcpy(this->pitch, (char *) data);
  } else if (id == 'Q') {
    strcpy(this->q, (char *) data);
  } else if (id == 't') {
    strcpy(this->twr, (char *) data);
  }
}
