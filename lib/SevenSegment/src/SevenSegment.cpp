#include "SevenSegment.h"

SevenSegment::SevenSegment() : 
    ledCtrl(A3, A1, A2, 1)
{
    this->mode = debug;
}

void SevenSegment::init(Telemetry* telemetry)
{
    this->lastMetUpdate = 0;
    this->lastMetValue = -1;
    this->lastValue = 0;
    this->telemetry = telemetry;

    this->ledCtrl.shutdown(0,false);
    this->ledCtrl.setIntensity(0,6);
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

void SevenSegment::_printMET(unsigned long value) {
    uint8_t seconds = (uint8_t) (value % 60);
    uint8_t minutes = (uint8_t)(value / 60 % 60);
    uint16_t hours = (uint16_t) (value / 3600);

    char string[5] = "\0\0\0\0";

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
}
