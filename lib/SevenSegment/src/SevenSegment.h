#pragma once

#include <Arduino.h>
#include <Telemetry.h>
#include <LedControl.h>

class SevenSegment {
    private:
        Telemetry* telemetry;
        LedControl ledCtrl;
        Mode mode;
        uint32_t lastValue;
        uint32_t lastMetValue;
        unsigned long lastMetUpdate;

    public:
        SevenSegment();
        void init(Telemetry* telemetry);
        void test();
        void refresh();
        void _printMET(uint32_t value);
};