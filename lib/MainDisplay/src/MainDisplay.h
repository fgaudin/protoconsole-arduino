#pragma once

#include <Arduino.h>
#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include <Controller.h>

enum DisplayMode {debug, ascent, orbit, descent};

class MainDisplay {
    private:
        Controller* controller;
        DisplayMode mode;
    public:
        MainDisplay();
        void init(Controller* controller);
        void setMode(DisplayMode mode);
        void refresh();
        void print(const char* str);
};