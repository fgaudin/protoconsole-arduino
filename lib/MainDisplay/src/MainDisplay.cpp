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
        this->lcd.print("VSp:000000");
        this->lcd.setCursor(0, i++);
        this->lcd.print("HSp:000000");
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
        metricfy(this->telemetry->apoapsis, data);
        padRight(' ', data, 7);
        this->lcd.print(data);

        memset(data, 0, 8);
        this->lcd.setCursor(3, 1);
        metricfy(this->telemetry->periapsis, data);
        padRight(' ', data, 7);
        this->lcd.print(data);

        memset(data, 0, 8);
        this->lcd.setCursor(4, 2);
        metricfy(this->telemetry->verticalSpeed , data);
        padRight(' ', data, 6);
        this->lcd.print(data);

        memset(data, 0, 8);
        this->lcd.setCursor(4, 3);
        metricfy(this->telemetry->horizontalSpeed , data);
        padRight(' ', data, 6);
        this->lcd.print(data);

        memset(data, 0, 8);
        this->lcd.setCursor(15, 0);
        metricfy(this->telemetry->altitude, data);
        padRight(' ', data, 5);
        this->lcd.print(data);

        memset(data, 0, 8);
        this->lcd.setCursor(15, 1);
        dtostrf(this->telemetry->twr, 5, 1, data);
        this->lcd.print(data);

        memset(data, 0, 8);
        this->lcd.setCursor(17, 2);
        ltoa(this->telemetry->pitch, data, 10);
        padRight(' ', data, 3);
        this->lcd.print(data);
    }
}

void MainDisplay::print(const char* str)
{
    this->lcd.print(str);
}
