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
    if (mode == this->mode) {
        return;
    }
    char leftArrow = 127;
    char rightArrow = 126;
    this->mode = mode;
    this->lcd.clear();

    if (this->mode == debug) {
        this->lcd.setCursor(19, 3);
        this->lcd.print(rightArrow);
        this->lcd.home();
    } else if (this->mode == ascent) {
        this->lcd.setCursor(0, 0);
        this->lcd.print(leftArrow);
        int row = 0;
        int colLeft = 1;
        int colRight = 10;
        this->lcd.setCursor(colLeft, row++);
        this->lcd.print("Ap:00000");
        this->lcd.setCursor(colLeft, row++);
        this->lcd.print("Pe:00000");
        this->lcd.setCursor(colLeft, row++);
        this->lcd.print("Vs:00000");
        this->lcd.setCursor(colLeft, row++);
        this->lcd.print("Hs:00000");
        row = 0;
        this->lcd.setCursor(colRight, row++);
        this->lcd.print("Alt:00000");
        this->lcd.setCursor(colRight, row++);
        this->lcd.print("TWR:00000");
        this->lcd.setCursor(colRight, row++);
        this->lcd.print("Pch:0000");
    }
}

void MainDisplay::refresh()
{
    if (this->mode == debug) {
        this->lcd.home();
        this->lcd.print(this->debug_str);
    } else if (this->mode == ascent) {
        int colLeft = 1;
        int colRight = 10;
        
        char data[8];

        memset(data, 0, 8);
        this->lcd.setCursor(colLeft+3, 0);
        metricfy(this->telemetry->apoapsis, data);
        padLeft(' ', data, 5);
        this->lcd.print(data);

        memset(data, 0, 8);
        this->lcd.setCursor(colLeft+3, 1);
        metricfy(this->telemetry->periapsis, data);
        padLeft(' ', data, 5);
        this->lcd.print(data);

        memset(data, 0, 8);
        this->lcd.setCursor(colLeft+3, 2);
        metricfy(this->telemetry->verticalSpeed , data);
        padLeft(' ', data, 5);
        this->lcd.print(data);

        memset(data, 0, 8);
        this->lcd.setCursor(colLeft+3, 3);
        metricfy(this->telemetry->horizontalSpeed , data);
        padLeft(' ', data, 5);
        this->lcd.print(data);

        memset(data, 0, 8);
        this->lcd.setCursor(colRight+4, 0);
        metricfy(this->telemetry->altitude, data);
        padLeft(' ', data, 5);
        this->lcd.print(data);

        memset(data, 0, 8);
        this->lcd.setCursor(colRight+4, 1);
        dtostrf(this->telemetry->twr, 5, 1, data);
        this->lcd.print(data);

        memset(data, 0, 8);
        this->lcd.setCursor(colRight+4, 2);
        ltoa(this->telemetry->pitch, data, 10);
        padLeft(' ', data, 5);
        this->lcd.print(data);
    }
}

void MainDisplay::print(const char* str)
{
    this->lcd.print(str);
}
