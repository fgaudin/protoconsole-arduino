#include "LedDisplay.h"

#define LED_SOLAR_OFF 7
#define LED_SOLAR_ON 4
#define LED_BREAK_OFF 6
#define LED_BREAK_ON 2
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

void LedDisplay::_on(int led) {
    this->ledCtrl.setLed(0, led >> 3, led & B111, true);
}

void LedDisplay::_off(int led) {
    this->ledCtrl.setLed(0, led >> 3, led & B111, false);
}

void LedDisplay::refresh()
{
    if (this->telemetry->solarPanel == 3) {
        this->_on(LED_SOLAR_ON);
        this->_off(LED_SOLAR_OFF);
    } else if (this->telemetry->solarPanel == 0) {
        this->_on(LED_SOLAR_OFF);
        this->_off(LED_SOLAR_ON);
    }
}