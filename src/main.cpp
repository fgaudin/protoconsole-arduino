#include <Arduino.h>
#include <SoftwareSerial.h>
#include <LedDisplay.h>
#include <Telemetry.h>
#include <BarGraph.h>
#include <MainDisplay.h>
#include <SevenSegment.h>

#define DEBUG 1
#define TEST 0

const int pinDebugRx = 6;
const int pinDebugTx = 7;

// leds
const int pinLedData = 11;
const int pinLedClock = 12;
const int pinLedLoad = 13;
// bars
const int pinBarData = 8;
const int pinBarClock = 9;
const int pinBarLoad = 10;

SoftwareSerial SerialDebug(pinDebugRx, pinDebugTx); // RX, TX

uint8_t read = 0;
const uint8_t max_packet_size = 128;
char received[max_packet_size] = "";

Telemetry telemetry;
LedDisplay leds;
BarGraph bars;
MainDisplay display;
SevenSegment seg7;

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
  bars.init(pinBarData, pinBarClock, pinBarLoad, &telemetry);

  // LCD
  display.init(&telemetry);
  strcpy(display.debug_str, "controller ready");
  display.setMode(ascent);
  display.refresh();

  // 7 segments
  seg7.init(&telemetry);
}

void handle_packet(char * packet) {
  uint8_t expected_size = 0;
  uint8_t payload_size;

  #ifdef DEBUG
  SerialDebug.println(packet);
  #endif

  bool byte_payload = false;
  bool refresh_bars = false;
  bool refresh_display = false;

  switch (packet[0]) {
    case 'f':  // 1 bit flags for leds
      expected_size = 1;  // in bytes, since hexa FF = 1 byte
      byte_payload = true;
      break;
    case 'g':  // 2 bits flags for leds
      expected_size = 1;
      byte_payload = true;
      break;
    case 'u':  // fuels
      expected_size = 5;
      byte_payload = true;
      refresh_bars = true;
      bars.setMode(fuel);
      break;
    case 'l':  // life support
      expected_size = 5;
      byte_payload = true;
      refresh_bars = true;
      bars.setMode(lifesupport);
      break;
    case 'a':  // apoapsis
      refresh_display = true;
      break;
    case 'p':  // periapsis
      refresh_display = true;
      break;
    case 'v':  // vertical speed
      refresh_display = true;
      break;
    case 'h':  // horizontal speed
      refresh_display = true;
      break;
    case 'A':  // altitude
      refresh_display = true;
      break;
    case 'P':  // pitch
      refresh_display = true;
      break;
    case 'Q':  // dynamic pressure
      refresh_display = true;
      break;
    case 't':  // twr
      refresh_display = true;
      break;
    case 'm':  // Mission Elapsed Time
      expected_size = 3;
      byte_payload = true;
      break;
  }

  if (byte_payload) {
    payload_size = (strlen(packet) - 2)/2;
  } else {
    payload_size = strlen(packet) + 1;
  }

  if (byte_payload && payload_size != expected_size) {
    #ifdef DEBUG
    SerialDebug.println("packet size incorrect");
    #endif
    return;
  }
  
  byte payload[(int)(payload_size)] = {0};

  if (byte_payload) {
    for(int i=0; i<payload_size;++i) {
      char c = packet[2*i+2];
      payload[i] = ((c >= 'A') ? c - 'A' + 10 : c - '0') << 4;
      c = packet[2*i+3];
      payload[i] |= (c >= 'A') ? c - 'A' + 10 : c - '0';
    }
  } else {
    strncpy((char *)payload, &(packet[2]), payload_size - 1);
    payload[payload_size] = '\0';
  }

  telemetry.update(packet[0], payload);

  if (refresh_bars) bars.refresh();
  if (refresh_display) {
    #ifdef DEBUG
    SerialDebug.println("refreshing display");
    #endif
    display.refresh();
  }
}

void loop() {
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
  seg7.refresh();
}