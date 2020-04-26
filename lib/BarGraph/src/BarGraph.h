#pragma once

#include <Arduino.h>
#include <Telemetry.h>

class BarGraph {
    private:
        int pinSerial;
        int pinLatch;
        int pinClock;
        int displayValue;
        int* telemetryData;
    public:
        BarGraph();
        void init(int  pinSerial, int pinLatch, int pinClock);
        void setSource(int* telemetryData);
        void refresh();
};