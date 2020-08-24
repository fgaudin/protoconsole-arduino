#include "LedDisplay.h"

#define LED_SOLAR_OFF 7
#define LED_SOLAR_ON 4
#define LED_BRAKE_OFF 6
#define LED_BRAKE_ON 2
#define LED_GEAR_OFF 5
#define LED_GEAR_ON 3
#define LED_005G 0
#define LED_CONTACT 1
#define LED_STAGE 14
#define LED_SAS 12
#define LED_RCS 13
#define LED_LIGHT 11
#define LED_DOCKED 10
#define LED_ANTENNA_GOOD 15
#define LED_ANTENNA_MEDIUM 9
#define LED_ANTENNA_BAD 8


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
    if (this->telemetry->solarPanel == 3) {
        this->_on(LED_SOLAR_ON);
        this->_off(LED_SOLAR_OFF);
    } else if (this->telemetry->solarPanel == 2) {
        this->_off(LED_SOLAR_OFF);
        this->_blink(LED_SOLAR_ON);
    } else if (this->telemetry->solarPanel == 1) {
        this->_blink(LED_SOLAR_OFF);
        this->_off(LED_SOLAR_ON);
    } else if (this->telemetry->solarPanel == 0) {
        this->_on(LED_SOLAR_OFF);
        this->_off(LED_SOLAR_ON);
    }

    if (this->telemetry->gear == 3) {
        this->_on(LED_GEAR_ON);
        this->_off(LED_GEAR_OFF);
    } else if (this->telemetry->gear == 2) {
        this->_off(LED_GEAR_OFF);
        this->_blink(LED_GEAR_ON);
    } else if (this->telemetry->gear == 1) {
        this->_blink(LED_GEAR_OFF);
        this->_off(LED_GEAR_ON);
    } else if (this->telemetry->gear == 0) {
        this->_on(LED_GEAR_OFF);
        this->_off(LED_GEAR_ON);
    }

    this->_toggle(LED_RCS, this->telemetry->rcs);
    this->_toggle(LED_SAS, this->telemetry->sas);
    this->_toggle(LED_BRAKE_ON, this->telemetry->brake);
    this->_toggle(LED_BRAKE_OFF, !this->telemetry->brake);
    this->_toggle(LED_DOCKED, this->telemetry->docked);
    this->_toggle(LED_LIGHT, this->telemetry->lights);
    this->_toggle(LED_005G, this->telemetry->dot05g);
    this->_toggle(LED_CONTACT, this->telemetry->contact);
}