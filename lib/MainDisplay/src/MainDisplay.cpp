#include "MainDisplay.h"

LiquidCrystal_PCF8574 lcd(0x27);

MainDisplay::MainDisplay()
{
    this->mode = debug;
}

void MainDisplay::init(Controller* controller)
{
    this->controller = controller;
    lcd.begin(20, 4); // initialize the lcd
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();
}

void MainDisplay::setMode(DisplayMode mode)
{
    this->mode = mode;
}

void MainDisplay::refresh()
{
    char row[20];
    if (this->mode == ascent) {
        lcd.clear();
        lcd.home();
        lcd.print(this->controller->debug_str);
    } else if (this->mode == ascent) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Pe:");
        ltoa(this->controller->periapsis, row, 10);
        lcd.print(row);
        lcd.setCursor(0, 1);
        lcd.print("Ap:");
        ltoa(this->controller->apoapsis, row, 10);
        lcd.print(row);
    }
}

void MainDisplay::print(const char* str)
{
    lcd.print(str);
}
