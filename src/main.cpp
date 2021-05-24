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

  // simpit
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
  mySimpit.registerChannel(CAGSTATUS_MESSAGE);
  mySimpit.registerChannel(APSIDES_MESSAGE);
  mySimpit.registerChannel(VELOCITY_MESSAGE);
  mySimpit.registerChannel(TACLS_RESOURCE_MESSAGE);
  mySimpit.registerChannel(TACLS_WASTE_MESSAGE);
  mySimpit.registerChannel(APSIDESTIME_MESSAGE);
  mySimpit.registerChannel(ORBIT_INFO);
  mySimpit.registerChannel(MANEUVER_MESSAGE);
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
        if (round(telemetry.altitude) != round(myAltitude.sealevel)) {
          telemetry.altitude = myAltitude.sealevel;
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
    case TACLS_RESOURCE_MESSAGE:
      if (msgSize == sizeof(TACLSResourceMessage)) {
        TACLSResourceMessage resource = parseTACLSResource(msg);
        telemetry.stageFood = (int) round(10 * resource.currentFood / resource.maxFood);
        telemetry.stageH2O = (int) round(10 * resource.currentWater / resource.maxWater);
        telemetry.stageO2 = (int) round(10 * resource.currentOxygen / resource.maxOxygen);
      }
    break;
    case TACLS_WASTE_MESSAGE:
      if (msgSize == sizeof(TACLSWasteMessage)) {
        TACLSWasteMessage resource = parseTACLSWaste(msg);
        telemetry.stageWaste = (int) round(10 * resource.currentWaste / resource.maxWaste);
        telemetry.stageCO2 = (int) round(10 * resource.currentCO2 / resource.maxCO2);
      }
    break;
    case FLIGHT_STATUS_MESSAGE:
      if (msgSize == sizeof(flightStatusMessage)) {
        flightStatusMessage flight = parseFlightStatusMessage(msg);
        telemetry.contact = (flight.vesselSituation <= 4);  // Landed: 1, splashed: 2, pre-launch: 4, flying: 8
        telemetry.antenna = (int) ceil(3.0 * flight.commNetSignalStrenghPercentage / 100.0);
      }
    break;
    case CAGSTATUS_MESSAGE:
      if (msgSize == sizeof(cagStatusMessage)) {
        cagStatusMessage status = parseCAGStatusMessage(msg);
        telemetry.solarPanel = status.is_action_activated(1);
        telemetry.stage = status.is_action_activated(242);
        if (status.is_action_activated(243)) {
          bars.setMode(lifesupport);
        } else {
          bars.setMode(fuel);
        }

        int mode = 0;
        if (status.is_action_activated(240)) {
          mode += 1;
        }
        if (status.is_action_activated(241)) {
          mode += 2;
        }
        switch(mode) {
          case 0:
            display.setMode(ascent);
          break;
          case 1:
            display.setMode(orbit);
          break;
          case 2:
            display.setMode(descent);
          break;
          case 3:
            display.setMode(docking);
          break;
        }
      }
    break;
    case APSIDES_MESSAGE:
      if (msgSize == sizeof(apsidesMessage)) {
        apsidesMessage apside = parseApsides(msg);
        telemetry.apoapsis = apside.apoapsis;
        telemetry.periapsis = apside.periapsis;
      }
    break;
    case VELOCITY_MESSAGE:
      if (msgSize == sizeof(velocityMessage)) {
        velocityMessage velocity = parseVelocity(msg);
        telemetry.verticalSpeed = velocity.vertical;
        telemetry.orbitalSpeed = velocity.orbital;
      }
    break;
    case APSIDESTIME_MESSAGE:
      if (msgSize == sizeof(apsidesTimeMessage)) {
        apsidesTimeMessage times = parseApsidesTime(msg);
        telemetry.apoapsisTime = times.apoapsis;
        telemetry.periapsisTime = times.periapsis;
      }
    break;
    case ORBIT_INFO:

    break;
    case MANEUVER_MESSAGE:

    break;
  }
}
