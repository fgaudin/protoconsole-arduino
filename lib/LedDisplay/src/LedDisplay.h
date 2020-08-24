#pragma once

#include <Arduino.h>
#include <Telemetry.h>
#include <LedControl.h>

class LedDisplay {
    private:
        int pinData;
        int pinClock;
        int pinLoad;
        Telemetry* telemetry;
        LedControl ledCtrl;

    public:
        LedDisplay();
        void init(int  pinData, int pinClock, int pinLoad, Telemetry* telemetry);
        void refresh();
        void _toggle(int led, bool val);
        void _on(int led);
        void _off(int led);
        void _blink(int led);
};