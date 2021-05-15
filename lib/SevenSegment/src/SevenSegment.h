#pragma once

#include <Arduino.h>
#include <Telemetry.h>
#include <LedControl.h>

class SevenSegment {
    private:
        Telemetry* telemetry;
        LedControl ledCtrl;
        Mode mode;
        long lastValue;
        long lastMetValue;
        long lastMetUpdate;

    public:
        SevenSegment();
        void init(Telemetry* telemetry);
        void test();
        void refresh();
        void _printValue(float value, int decimals);
        void _printMET(long value);
        void setMode(Mode mode);
};