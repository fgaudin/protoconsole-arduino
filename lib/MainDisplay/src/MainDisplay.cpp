#include "MainDisplay.h"

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
}

void MainDisplay::refresh()
{
    char row[20];
    if (this->mode == debug) {
        this->lcd.clear();
        this->lcd.home();
        this->lcd.print(this->debug_str);
    } else if (this->mode == ascent) {
        this->lcd.clear();
        this->lcd.setCursor(0, 0);
        this->lcd.print("Pe:");
        ltoa(this->telemetry->periapsis, row, 10);
        this->lcd.print(row);
        this->lcd.setCursor(0, 1);
        this->lcd.print("Ap:");
        ltoa(this->telemetry->apoapsis, row, 10);
        this->lcd.print(row);
    }
}

void MainDisplay::print(const char* str)
{
    this->lcd.print(str);
}
