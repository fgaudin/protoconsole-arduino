#include <Arduino.h>
#include <Controller.h>


bool connected = false;
Controller controller;

void setup()
{
  Serial.begin(115200);
  controller.init();
}

void hello()
{
  Serial.write(1);
}

void listen()
{
  if(Serial.available() > 0) {
    int cmd = Serial.read();
    int value_size = 0;
    byte value[4];

    if (cmd < 64) {
    } else if (cmd < 128) {
      value_size = 1;
    } else if (cmd < 192) {
      value_size = 2;
    } else {
      value_size = 4;
    }
    
    int read = Serial.readBytes(value, value_size);

    if (cmd > 0 && cmd < 256) {
      controller.handle_command(cmd, value);
    } else {
      //debug("command out of range");
    }
  }
}

void normal_loop()
{
  if (!controller.connected) {
    hello();
    delay(200);
  }
  
  listen();
  controller.update();
}

void loop() {
  normal_loop();
}