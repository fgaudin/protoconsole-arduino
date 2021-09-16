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

const int pinLSMode = 4;
const int pinFuelMode = 5;

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

// SoftwareSerial Serial1(pinDebugRx, pinDebugTx); // RX, TX

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
  // Serial1.begin(38400);

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

  #if TEST
  return;
  #endif

  // simpit
  while (!mySimpit.init()) {
    delay(100);
  }
  mySimpit.printToKSP("Display Connected", PRINT_TO_SCREEN);

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
  mySimpit.registerChannel(DELTAV_MESSAGE);
}

int test_step = 19;
int test_delay = 800;

void test_mode() {
  delay(test_delay);
  switch(test_step) {
    case 0:
      display.setMode(debug);
      strcpy(display.debug_str, "testing mode");
    break;
    case 1:
      telemetry.lights = true;
      strcpy(display.debug_str, "lights on");
    break;
    case 2:
      telemetry.docked = true;
      strcpy(display.debug_str, "docked on");
    break;
    case 3:
      telemetry.rcs = true;
      strcpy(display.debug_str, "rcs on");
    break;
    case 4:
      telemetry.sas = true;
      strcpy(display.debug_str, "sas on");
    break;
    case 5:
      telemetry.antenna = 1;
      strcpy(display.debug_str, "antenna 1");
    break;
    case 6:
      telemetry.antenna = 2;
      strcpy(display.debug_str, "antenna 2");
    break;
    case 7:
      telemetry.antenna = 3;
      strcpy(display.debug_str, "antenna 3");
    break;
    case 8:
      telemetry.contact = true;
      strcpy(display.debug_str, "contact on");
    break;
    case 9:
      telemetry.dot05g = true;
      strcpy(display.debug_str, ".05g on");
    break;
    case 10:
      telemetry.dot05g = true;
      strcpy(display.debug_str, ".05g on");
    break;
    case 11:
      telemetry.master_alarm = true;
      strcpy(display.debug_str, "master alarm on");
    break;
    case 12:
      telemetry.solarPanel = true;
      strcpy(display.debug_str, "solar on");
    break;
    case 13:
      telemetry.gear = true;
      strcpy(display.debug_str, "gear on");
    break;
    case 14:
      telemetry.brake = true;
      strcpy(display.debug_str, "break on");
    break;
    case 15:
      telemetry.stage = true;
      strcpy(display.debug_str, "stage on");
    break;
    case 16:
      telemetry.stageFuel++;
      telemetry.stageOx++;
      telemetry.stageMonoprop++;
      telemetry.stageElec++;
      telemetry.stageXenon++;
      strcpy(display.debug_str, "testing led bars");
      if(telemetry.stageFuel < 10) {
        test_step--;
      }
      test_delay = 200;
    break;
    case 17:
      if (telemetry.met < 5*4000271) telemetry.met += 4000271;  // 3600 * 1111 + 60 * 11 + 11
      else telemetry.met += 3999600;  // 3600*1111
      strcpy(display.debug_str, "testing 7 segments");
      test_delay = 500;
      if (telemetry.met <= 36002439) test_step--;
      else telemetry.met = 0;
    break;
    case 18:
      test_delay = 300;
      if (telemetry.apoapsis == 0) display.setMode(ascent);
      telemetry.apoapsis = 2 * (1 + telemetry.apoapsis);
      telemetry.periapsis = 2 * (1 + telemetry.periapsis);
      telemetry.verticalSpeed = 2 * (1 + telemetry.verticalSpeed);
      telemetry.orbitalSpeed = 2 * (1 + telemetry.verticalSpeed);
      telemetry.altitude = 2 * (1 + telemetry.altitude);
      telemetry.pitch = (telemetry.pitch + 9) % 360;
      if (telemetry.apoapsis < pow(10, 16)) test_step--;
    break;
    case 19:
      test_delay = 300;
      if (telemetry.timeToNextManeuver == 0) {
        display.setMode(orbit);
        telemetry.apoapsis = 0;
        telemetry.periapsis = 0;
      }
      telemetry.apoapsis = 2 * (1 + telemetry.apoapsis);
      telemetry.periapsis = 2 * (1 + telemetry.periapsis);
      telemetry.apoapsisTime += 59.0;
      telemetry.periapsisTime += 59.0;
      telemetry.timeToNextManeuver += 59.0;
      telemetry.inclination += 1;
      telemetry.deltav += 111;
      if (telemetry.timeToNextManeuver < pow(10, 16)) test_step--;
    break;
    default:
      test_step = 0;
    break;
  }

  display.refresh();
  test_step++;
}

void loop() {
  #if TEST == 0
  mySimpit.update();
  #else
  test_mode();
  #endif
  leds.refresh();
  display.update();
  seg7.refresh();

  if (digitalRead(pinLSMode)) {
    bars.setMode(lifesupport);
  } else {
    bars.setMode(fuel);
  }
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
      } else {
        mySimpit.printToKSP("wrong altitude message size", VERBOSE_ONLY);
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
        display.refresh();
      }
    break;
    case VELOCITY_MESSAGE:
      if (msgSize == sizeof(velocityMessage)) {
        velocityMessage velocity = parseVelocity(msg);
        telemetry.verticalSpeed = velocity.vertical;
        telemetry.orbitalSpeed = velocity.orbital;
        display.refresh();
      }
    break;
    case APSIDESTIME_MESSAGE:
      if (msgSize == sizeof(apsidesTimeMessage)) {
        apsidesTimeMessage times = parseApsidesTime(msg);
        telemetry.apoapsisTime = times.apoapsis;
        telemetry.periapsisTime = times.periapsis;
        display.refresh();
      }
    break;
    case ORBIT_INFO:
       if (msgSize == sizeof(orbitInfoMessage)) {
        orbitInfoMessage orbitInfo = parseOrbitInfo(msg);
        telemetry.inclination = orbitInfo.inclination;
        display.refresh();
      }
    break;
    case MANEUVER_MESSAGE:
      if (msgSize == sizeof(maneuverMessage)) {
        maneuverMessage times = parseManeuver(msg);
        telemetry.timeToNextManeuver = times.timeToNextManeuver;
        display.refresh();
      }
    break;
    case DELTAV_MESSAGE:
       if (msgSize == sizeof(deltaVMessage)) {
        deltaVMessage dv = parseDeltaV(msg);
        telemetry.deltav = dv.stageDeltaV;
        display.refresh();
      }
    break;
  }
}
