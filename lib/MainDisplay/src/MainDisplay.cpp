#include "MainDisplay.h"
#include <Utils.h>

MainDisplay::MainDisplay() : lcd(0x27)
{
    this->mode = debug;
}

void MainDisplay::init(Telemetry* telemetry)
{
    this->telemetry = telemetry;
    this->lcd.begin(20, 4);
    this->lcd.setBacklight(255);
    this->lcd.home();
    this->lcd.clear();
    this->lcd.print("Display initialized");
}

void MainDisplay::setMode(DisplayMode mode)
{
    this->mode = mode;
    this->lcd.clear();

    if (this->mode == debug) {
        this->lcd.home();
    } else if (this->mode == ascent) {
        int i = 0;
        this->lcd.setCursor(0, i++);
        this->lcd.print("Ap:0000000");
        this->lcd.setCursor(0, i++);
        this->lcd.print("Pe:0000000");
        this->lcd.setCursor(0, i++);
        this->lcd.print("vVel:00000");
        this->lcd.setCursor(0, i++);
        this->lcd.print("hVel:00000");
        i = 0;
        this->lcd.setCursor(11, i++);
        this->lcd.print("Alt:00000");
        this->lcd.setCursor(11, i++);
        this->lcd.print("TWR:00000");
        this->lcd.setCursor(11, i++);
        this->lcd.print("Pitch:000");
    }
}

void MainDisplay::refresh()
{
    if (this->mode == debug) {
        this->lcd.home();
        this->lcd.print(this->debug_str);
    } else if (this->mode == ascent) {
        char data[8];

        this->lcd.setCursor(3, 0);
        ltoa(this->telemetry->apoapsis, data, 10);
        padRight(' ', data, sizeof(data) - 1);
        this->lcd.print(data);

        this->lcd.setCursor(3, 1);
        ltoa(this->telemetry->periapsis, data, 10);
        padRight(' ', data, sizeof(data) - 1);
        this->lcd.print(data);
    }
}

void MainDisplay::print(const char* str)
{
    this->lcd.print(str);
}
