#pragma once

#include <Arduino.h>
#include <Telemetry.h>
#include <LedControl.h>

enum BarGraphMode {lifesupport, fuel};

class BarGraph {
    private:
        int pinData;
        int pinClock;
        int pinLoad;
        Telemetry* telemetry;
        LedControl ledCtrl;
        int bar[5] = {10, 10, 10, 10, 10};
        // 5 bars, 10 leds, column & row
        byte addr[5][10][2];
        BarGraphMode mode;

    public:
        BarGraph();
        void init(int  pinData, int pinClock, int pinLoad, Telemetry* telemetry);
        void setMode(BarGraphMode mode);
        void refresh();
        void test();
};