#pragma once

#include <Arduino.h>
#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include <Telemetry.h>

class MainDisplay {
    private:
        Telemetry* telemetry;
        Mode mode;
        LiquidCrystal_PCF8574 lcd;
        bool needsUpdate;
        long lastUpdate;
    public:
        char debug_str[80];
        MainDisplay();
        void init(Telemetry* telemetry);
        void setMode(Mode mode);
        void refresh();
        void update();
        void print(const char* str);
};
