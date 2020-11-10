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

void SevenSegment::refresh()
{
    if (this->mode == debug) {
        for (int i=0; i<8; i++) {
            this->ledCtrl.setDigit(0,i,8,true);
        }
    } else if (this->mode == ascent) {
        for (int i=0; i<8; i++) {
            int value = (this->telemetry->horizontalSpeed / i) % 10;
            this->ledCtrl.setDigit(0,i,value,false);
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