#include "SevenSegment.h"

SevenSegment::SevenSegment() : 
    ledCtrl(A3, A1, A2, 1)
{
    this->mode = debug;
}

void SevenSegment::init(Telemetry* telemetry)
{
    this->telemetry = telemetry;

    this->ledCtrl.shutdown(0,false);
    this->ledCtrl.setIntensity(0,1);
    this->ledCtrl.clearDisplay(0);
}

void SevenSegment::test() {
    for (int i=0; i<=15; i++) {
        for (int j=0; j<=7; j++) {
            this->ledCtrl.setDigit(0,j,i,false);
            delay(50);
        }
        delay(1000);
        this->ledCtrl.clearDisplay(0);
    }    
    this->ledCtrl.clearDisplay(0);
}

void SevenSegment::_printValue(float value, int decimals) {
    char string[10];
    dtostrf(value, 9, decimals, string);

    unsigned int i = 0;
    bool dp = false;
    char digit;

    this->ledCtrl.clearDisplay(0);
    for (int j=0; j<8; j++) {
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

void SevenSegment::refresh()
{
    if (this->mode == debug) {
        this->_printValue(88888888, 0);
    } else if (this->mode == ascent) {
        if (this->telemetry->altitude < 3000) {
            this->_printValue(1, 0);
        } else {
            this->_printValue(2, 0);
        }
    }
}

void SevenSegment::setMode(Mode mode)
{
    if (mode == this->mode) {
        return;
    }
    this->mode = mode;
}