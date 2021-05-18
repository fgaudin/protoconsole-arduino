#pragma once

#include <Arduino.h>
#include <Telemetry.h>
#include <LedControl.h>

class SevenSegment {
    private:
        Telemetry* telemetry;
        LedControl ledCtrl;
        Mode mode;
        unsigned long lastValue;
        unsigned long lastMetValue;
        unsigned long lastMetUpdate;

    public:
        SevenSegment();
        void init(Telemetry* telemetry);
        void test();
        void refresh();
        void _printMET(unsigned long value);
};