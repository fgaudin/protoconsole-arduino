#include "BarGraph.h"

BarGraph::BarGraph()
{
    this->displayValue = 0;
}

void BarGraph::init(int  pinSerial, int pinLatch, int pinClock)
{
    this->pinSerial = pinSerial;
    this->pinLatch = pinLatch;
    this->pinClock = pinClock;
    pinMode(pinSerial, OUTPUT);
    pinMode(pinLatch, OUTPUT);
    pinMode(pinClock, OUTPUT);
}

void BarGraph::setSource(int* telemetryData)
{
    this->telemetryData = telemetryData;
}

void BarGraph::refresh()
{
    long value = round(*this->telemetryData/10.0);
    digitalWrite(this->pinLatch, LOW);
    digitalWrite(this->pinSerial, HIGH);
    for (int i=0; i<value; i++) {
        digitalWrite(this->pinClock, LOW);
        digitalWrite(this->pinClock, HIGH);
    }
    digitalWrite(this->pinSerial, LOW);
    for (int i=0; i<10-value; i++) {
        digitalWrite(this->pinClock, LOW);
        digitalWrite(this->pinClock, HIGH);
    }
    digitalWrite(this->pinLatch, HIGH);
}