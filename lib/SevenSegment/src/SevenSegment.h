#pragma once

#include <Arduino.h>
#include <Telemetry.h>
#include <LedControl.h>

class SevenSegment {
    private:
        Telemetry* telemetry;
        LedControl ledCtrl;

    public:
        SevenSegment();
        void init(Telemetry* telemetry);
        void test();
        void refresh();
};