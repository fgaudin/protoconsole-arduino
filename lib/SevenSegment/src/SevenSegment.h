#pragma once

#include <Arduino.h>
#include <Telemetry.h>
#include <LedControl.h>

class SevenSegment {
    private:
        Telemetry* telemetry;
        LedControl ledCtrl;
        Mode mode;

    public:
        SevenSegment();
        void init(Telemetry* telemetry);
        void test();
        void refresh();
        void setMode(Mode mode);
};