#include <Arduino.h>
#include <SoftwareSerial.h>
#include <LedDisplay.h>
#include <Telemetry.h>

#define DEBUG 1
#define TEST 0

const int pinDebugRx = 6;
const int pinDebugTx = 7;

// leds
const int pinLedData = 11;
const int pinLedClock = 12;
const int pinLedLoad = 13;

SoftwareSerial SerialDebug(pinDebugRx, pinDebugTx); // RX, TX

uint8_t read = 0;
const uint8_t max_packet_size = 128;
char received[max_packet_size] = "";

Telemetry telemetry;
LedDisplay leds;

//bool connected = false;
//Controller controller;

void setup()
{
  #ifdef DEBUG
  SerialDebug.begin(38400);
  SerialDebug.println("Starting up");
  #endif

  // data link
  Serial.begin(57600);
  Serial.print("!");

  // leds
  leds.init(pinLedData, pinLedClock, pinLedLoad, &telemetry);

  //controller.init();
}

// deprecated
// void hello()
// {
//   Serial.write(1);
// }

// // deprecated
// void listen()
// {
//   if(Serial.available() > 0) {
//     int cmd = Serial.read();
//     int value_size = 0;
//     byte value[4];

//     if (cmd < 8) {
//     } else if (cmd < 32) {
//       value_size = 1;
//     } else {
//       value_size = 4;
//     }
    
//     Serial.readBytes(value, value_size);

//     if (cmd > 0 && cmd < 64) {
//       controller.handle_command(cmd, value);
//     } else {
//       //debug("command out of range");
//     }
//   }
// }

// // deprecated
// void normal_loop()
// {
//   if (!controller.connected) {
//     hello();
//     delay(200);
//   }
  
//   listen();
//   controller.update();
// }

void handle_packet(char * packet) {
  uint8_t payload_size = strlen(packet) - 2;
  uint8_t expected_size = 0;

  #ifdef DEBUG
  SerialDebug.println(packet);
  #endif

  switch (packet[0]) {
    case 'f':  // 1 bit flags for leds
      expected_size = 2;
      break;
    case 'g':  // 2 bits flags for leds
      expected_size = 2;
      break;
  }
  if (payload_size != expected_size) {
    #ifdef DEBUG
    SerialDebug.println("packed size incorrect");
    #endif
    return;
  }
  
  byte payload[(int)(payload_size/2)] = {0};

  for(int i=0; i<payload_size/2;++i) {
    char c = packet[2*i+2];
    payload[i] = ((c >= 'A') ? c - 'A' + 10 : c - '0') << 4;
    c = packet[2*i+3];
    payload[i] |= (c >= 'A') ? c - 'A' + 10 : c - '0';
  }

  telemetry.update(packet[0], payload);
}

void loop() {
  // if (TEST) {
  //   controller.test();
  // } else {
  //   normal_loop();
  // }
  char c = 0;

  if(Serial.available() > 0) {
    c = Serial.read();
    if (c == '[') {
      read = 0;
      received[0] = '\0';
    } else if (c == ']') {
      received[read] = '\0';
      read = 0;
      handle_packet(received);
    } else {
      if (read >= max_packet_size) {
        #ifdef DEBUG
          SerialDebug.println("packet too long");
        #endif
      } else {
        received[read] = c;
        read++;
      }
    }
  }

  leds.refresh();
}