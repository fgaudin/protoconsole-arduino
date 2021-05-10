#pragma once

#include <Arduino.h>
#include <LedControl.h>
#include <Telemetry.h>

class LedDisplay {
    private:
        int pinData;
        int pinClock;
        int pinLoad;
        LedControl ledCtrl;
        Telemetry* telemetry;

    public:
        LedDisplay();
        void init(int  pinData, int pinClock, int pinLoad, Telemetry* telemetry);
        void test();
        void refresh();
        void _toggle(int led, bool val);
        void _on(int led);
        void _off(int led);
        void _blink(int led);
};