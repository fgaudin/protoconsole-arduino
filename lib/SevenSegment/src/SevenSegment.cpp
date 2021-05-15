#include "SevenSegment.h"

SevenSegment::SevenSegment() : 
    ledCtrl(A3, A1, A2, 1)
{
    this->mode = debug;
}

void SevenSegment::init(Telemetry* telemetry)
{
    this->lastMetUpdate = 0;
    this->lastMetValue = 0;
    this->lastValue = 0;
    this->telemetry = telemetry;

    this->ledCtrl.shutdown(0,false);
    this->ledCtrl.setIntensity(0,1);
    this->ledCtrl.clearDisplay(0);
}

void SevenSegment::test() {
    for (int i=0; i<=15; i++) {
        for (int j=0; j<=7; j++) {
            this->ledCtrl.setDigit(0,j,i,true);
            delay(50);
        }
        delay(100);
        this->ledCtrl.clearDisplay(0);
    }    
    this->ledCtrl.clearDisplay(0);
}

void SevenSegment::_printValue(float value, int decimals) {
    char string[34];
    dtostrf(value, 33, decimals, string);

    unsigned int i = 0;
    bool dp = false;
    char digit;

    this->ledCtrl.clearDisplay(0);
    for (unsigned int j=0; j<strlen(string); j++) {
        digit = string[strlen(string) - i - 1];
        i++;
        if (digit == '.') {
            dp = true;
            digit = string[strlen(string) - i - 1];
            i++;
        } else {
            dp = false;
        }
        this->ledCtrl.setChar(0, j, digit, dp);
    }
}

void SevenSegment::_printMET(long value) {
    uint8_t seconds = value % 60;
    uint8_t minutes = ((int)floor(value / 60)) % 60;
    uint16_t hours = floor(value / 3600);

    char string[5] = "\0\0\0\0";

    this->ledCtrl.clearDisplay(0);
    sprintf(string, "%02d", seconds);
    this->ledCtrl.setChar(0, 0, string[1], false);
    this->ledCtrl.setChar(0, 1, string[0], false);

    sprintf(string, "%02d", minutes);
    this->ledCtrl.setChar(0, 2, string[1], true);
    this->ledCtrl.setChar(0, 3, string[0], false);

    sprintf(string, "%04d", hours);
    this->ledCtrl.setChar(0, 4, string[3], true);
    this->ledCtrl.setChar(0, 5, string[2], false);
    this->ledCtrl.setChar(0, 6, string[1], false);
    this->ledCtrl.setChar(0, 7, string[0], false);
}

void SevenSegment::refresh()
{
    if (this->telemetry->met != this->lastMetValue) {
        this->_printMET(this->telemetry->met);
        this->lastValue = this->telemetry->met;
        this->lastMetValue = this->telemetry->met;
        this->lastMetUpdate = millis();
    } else if (this->telemetry->met > 0) {
        long delta = floor((millis() - lastMetUpdate) / 1000);
        if (this->telemetry->met + delta - this->lastValue >= 1) {
            this->lastValue = this->telemetry->met + delta;
            this->_printMET(this->lastValue);
        }
    }
    
    // if (this->mode == debug) {
    //     this->_printValue((long)millis() * 1000, 0);
    // } else if (this->mode == ascent) {
    //     if (this->telemetry->altitude < 3000) {
    //         this->_printValue(this->telemetry->altitude, 0);
    //     } else if (this->telemetry->apoapsis < 70000) {
    //         this->_printValue(this->telemetry->apoapsis, 0);
    //     } else {
    //         this->_printValue(this->telemetry->verticalSpeed, 0);
    //     }
    // }
}

void SevenSegment::setMode(Mode mode)
{
    if (mode == this->mode) {
        return;
    }
    this->mode = mode;
}