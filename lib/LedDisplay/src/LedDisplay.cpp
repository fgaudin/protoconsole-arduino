#include "LedDisplay.h"

#define LED_SOLAR_OFF 12
#define LED_SOLAR_ON 8
#define LED_BRAKE_OFF 15
#define LED_BRAKE_ON 10
#define LED_GEAR_OFF 13
#define LED_GEAR_ON 11
#define LED_005G 9
#define LED_CONTACT 14
#define LED_STAGE 1
#define LED_SAS 2
#define LED_RCS 6
#define LED_LIGHT 3
#define LED_DOCKED 7
#define LED_ANTENNA_GOOD 5
#define LED_ANTENNA_MEDIUM 0
#define LED_ANTENNA_BAD 4
#define LED_MASTER_ALARM 16


LedDisplay::LedDisplay() : 
    ledCtrl(11, 12, 13, 1)
{

}

void LedDisplay::init(int  pinData, int pinClock, int pinLoad, Telemetry* telemetry)
{
    this->pinData = pinData;
    this->pinClock = pinClock;
    this->pinLoad = pinLoad;
    pinMode(pinData, OUTPUT);
    pinMode(pinClock, OUTPUT);
    pinMode(pinLoad, OUTPUT);
    this->telemetry = telemetry;

    this->ledCtrl.shutdown(0,false);
    this->ledCtrl.setIntensity(0,15);
}

void LedDisplay::test() {
    for (int i=0; i<=16; i++) {
        this->_on(i);
        delay(3000);
        this->_off(i);
    }
}

void LedDisplay::_toggle(int led, bool val) {
    this->ledCtrl.setLed(0, led >> 3, led & B111, val);
}

void LedDisplay::_on(int led) {
    this->_toggle(led, true);
}

void LedDisplay::_off(int led) {
    this->_toggle(led, false);
}

void LedDisplay::_blink(int led) {
    unsigned long now = micros();
    if ((now / 500000) % 2) {
        this->_on(led);
    } else {
        this->_off(led);
    }
}

void LedDisplay::refresh()
{
    switch (this->telemetry->solarPanel) {
        case 3:
            this->_on(LED_SOLAR_ON);
            this->_off(LED_SOLAR_OFF);
            break;
        case 2:
            this->_off(LED_SOLAR_OFF);
            this->_blink(LED_SOLAR_ON);
            break;
        case 1:
            this->_blink(LED_SOLAR_OFF);
            this->_off(LED_SOLAR_ON);
            break;
        case 0:
            this->_on(LED_SOLAR_OFF);
            this->_off(LED_SOLAR_ON);
            break;
    }

    switch (this->telemetry->gear) {
        case 3:
            this->_on(LED_GEAR_ON);
            this->_off(LED_GEAR_OFF);
            break;
        case 2:
            this->_off(LED_GEAR_OFF);
            this->_blink(LED_GEAR_ON);
            break;
        case 1:
            this->_blink(LED_GEAR_OFF);
            this->_off(LED_GEAR_ON);
            break;
        case 0:
            this->_on(LED_GEAR_OFF);
            this->_off(LED_GEAR_ON);
            break;
    }

    switch (this->telemetry->antenna) {
        case 3:
            this->_on(LED_ANTENNA_GOOD);
            this->_on(LED_ANTENNA_MEDIUM);
            this->_on(LED_ANTENNA_BAD);
            break;
        case 2:
            this->_off(LED_ANTENNA_GOOD);
            this->_on(LED_ANTENNA_MEDIUM);
            this->_on(LED_ANTENNA_BAD);
            break;
        case 1:
            this->_off(LED_ANTENNA_GOOD);
            this->_off(LED_ANTENNA_MEDIUM);
            this->_on(LED_ANTENNA_BAD);
            break;
        default:
            this->_off(LED_ANTENNA_GOOD);
            this->_off(LED_ANTENNA_MEDIUM);
            this->_off(LED_ANTENNA_BAD);
            break;
    }

    this->_toggle(LED_RCS, this->telemetry->rcs);
    this->_toggle(LED_SAS, this->telemetry->sas);
    this->_toggle(LED_BRAKE_ON, this->telemetry->brake);
    this->_toggle(LED_BRAKE_OFF, !this->telemetry->brake);
    this->_toggle(LED_DOCKED, this->telemetry->docked);
    this->_toggle(LED_LIGHT, this->telemetry->lights);
    this->_toggle(LED_005G, this->telemetry->dot05g);
    this->_toggle(LED_CONTACT, this->telemetry->contact);
    this->_toggle(LED_STAGE, this->telemetry->stage);

    if (this->telemetry->master_alarm) {
        this->_blink(LED_MASTER_ALARM);
    } else {
        this->_off(LED_MASTER_ALARM);
    }
}