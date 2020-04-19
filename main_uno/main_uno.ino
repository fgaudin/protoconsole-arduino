#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>

LiquidCrystal_PCF8574 lcd(0x27); // set the LCD address to 0x27 for a 16 chars and 2 line display



void setup()
{
  Serial.begin(115200);
  lcd.begin(20, 4); // initialize the lcd
  lcd.setBacklight(255);
  lcd.home();
  lcd.clear();
  lcd.print("Ready");
  delay(2000);
  lcd.clear();
  lcd.home();
}

void write_line(int row, char * str, bool clear)
{
  if (clear) {
    lcd.clear();
  }
  lcd.setCursor(0, row);
  lcd.print(str);
}


void loop()
{
  if(Serial.available() > 0) {
    write_line(0, "Data received", true);
    delay(200);
    char data = Serial.read();
    lcd.setCursor(0, 1);
    lcd.print("command: ");
    lcd.print(data);
    delay(500);
  }
}
