#pragma once

#include <Arduino.h>
#include <Telemetry.h>
#include <LedControl.h>

class BarGraph {
    private:
        int pinData;
        int pinClock;
        int pinLoad;
        Telemetry* telemetry;
        LedControl ledCtrl;
        int bar[5] = {0, 3, 5, 7, 10};
        // 5 bars, 10 leds, column & row
        byte addr[5][10][2];
    public:
        BarGraph();
        void init(int  pinData, int pinClock, int pinLoad, Telemetry* telemetry);
        void refresh();
};