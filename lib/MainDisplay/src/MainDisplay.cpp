#include "MainDisplay.h"
#include <Utils.h>

MainDisplay::MainDisplay() : lcd(0x27)
{
    this->mode = debug;
    this->needsUpdate = false;
    this->lastUpdate = 0;
}

void MainDisplay::init(Telemetry* telemetry)
{
    this->telemetry = telemetry;
    this->lcd.begin(20, 4);
    this->lcd.setBacklight(255);
    this->lcd.home();
    this->lcd.clear();
    this->lcd.print("Display initialized");
    delay(500);
}

void MainDisplay::setMode(Mode mode)
{
    if (mode == this->mode) {
        return;
    }

    this->needsUpdate = true;

    char leftArrow = 127;
    this->mode = mode;
    this->lcd.clear();

    if (this->mode == debug) {
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
        this->lcd.print("Os:00000");
        row = 0;
        this->lcd.setCursor(colRight, row++);
        this->lcd.print("Alt:00000");
        this->lcd.setCursor(colRight, row++);
        this->lcd.print("TWR:00000");
        this->lcd.setCursor(colRight, row++);
        this->lcd.print("Pch:00000");
        this->lcd.setCursor(colRight, row++);
        this->lcd.print("  Q:00000");
    } else if (this->mode == orbit) {
        this->lcd.setCursor(0, 1);
        this->lcd.print(leftArrow);
    } else if (this->mode == descent) {
        this->lcd.setCursor(0, 2);
        this->lcd.print(leftArrow);
    } else if (this->mode == docking) {
        this->lcd.setCursor(0, 3);
        this->lcd.print(leftArrow);
    }
}

void MainDisplay::refresh()
{
    this->needsUpdate = true;
}

void MainDisplay::update()
{
    if (!this->needsUpdate) return;

    if (millis() - this->lastUpdate < 300) return;

    if (this->mode == debug) {
        this->lcd.home();
        this->lcd.clear();
        this->lcd.print(this->debug_str);
    } else if (this->mode == ascent) {
        int colLeft = 1;
        int colRight = 10;
        
        char data[6];

        
        memset(data, 0, 6);
        metricfy((long) round(this->telemetry->apoapsis), data);
        padLeft(' ', data, 5);
        this->lcd.setCursor(colLeft+3, 0);
        this->lcd.print(data);

        memset(data, 0, 6);
        metricfy((long) round(this->telemetry->periapsis), data);
        padLeft(' ', data, 5);
        this->lcd.setCursor(colLeft+3, 1);
        this->lcd.print(data);
        
        memset(data, 0, 6);
        metricfy((long) round(this->telemetry->verticalSpeed), data);
        padLeft(' ', data, 5);
        this->lcd.setCursor(colLeft+3, 2);
        this->lcd.print(data);

        memset(data, 0, 6);
        metricfy((long) round(this->telemetry->orbitalSpeed), data);
        padLeft(' ', data, 5);
        this->lcd.setCursor(colLeft+3, 3);
        this->lcd.print(data);

        memset(data, 0, 6);
        metricfy((long) round(this->telemetry->altitude), data);
        padLeft(' ', data, 5);
        this->lcd.setCursor(colRight+4, 0);
        this->lcd.print(data);

        this->lcd.setCursor(colRight+4, 1);
        padLeft(' ', this->telemetry->twr, 5);
        this->lcd.print(this->telemetry->twr);

        memset(data, 0, 6);
        itoa((int)round(this->telemetry->pitch), data, 10);
        padLeft(' ', data, 5);
        this->lcd.setCursor(colRight+4, 2);
        this->lcd.print(data);

        this->lcd.setCursor(colRight+4, 3);
        padLeft(' ', this->telemetry->q, 5);
        this->lcd.print(this->telemetry->q);
    }
    this->lastUpdate = millis();
    this->needsUpdate = false;
}

void MainDisplay::print(const char* str)
{
    this->lcd.print(str);
}
