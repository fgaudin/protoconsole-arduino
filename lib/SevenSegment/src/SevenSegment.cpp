#include "SevenSegment.h"

SevenSegment::SevenSegment() : 
    ledCtrl(A3, A1, A2, 1)
{

}

void SevenSegment::init(Telemetry* telemetry)
{
    this->telemetry = telemetry;

    this->ledCtrl.shutdown(0,false);
    this->ledCtrl.setIntensity(0,1);
    this->ledCtrl.clearDisplay(0);
}

void SevenSegment::test() {
    for (int i=0; i<=9; i++) {
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
    
}