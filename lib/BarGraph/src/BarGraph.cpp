#include "BarGraph.h"

BarGraph::BarGraph() : 
    ledCtrl(8, 9, 10, 1),
    addr{
        {{6,7},{5,7},{0,7},{0,6},{0,5},{0,4},{0,3},{0,2},{0,1},{0,0}},
        {{6,6},{5,6},{1,7},{1,6},{1,5},{1,4},{1,3},{1,2},{1,1},{1,0}},
        {{6,5},{5,5},{2,7},{2,6},{2,5},{2,4},{2,3},{2,2},{2,1},{2,0}},
        {{6,4},{5,4},{3,7},{3,6},{3,5},{3,4},{3,3},{3,2},{3,1},{3,0}},
        {{6,3},{5,3},{4,7},{4,6},{4,5},{4,4},{4,3},{4,2},{4,1},{4,0}}
    }
{

}

void BarGraph::init(int  pinData, int pinClock, int pinLoad, Telemetry* telemetry)
{
    this->pinData = pinData;
    this->pinClock = pinClock;
    this->pinLoad = pinLoad;
    pinMode(pinData, OUTPUT);
    pinMode(pinClock, OUTPUT);
    pinMode(pinLoad, OUTPUT);
    this->telemetry = telemetry;

    this->ledCtrl.shutdown(0,false);
    this->ledCtrl.setIntensity(0,15);
    this->mode = fuel;
}

void BarGraph::test() {
    for (unsigned int b=0; b<5; b++) {
        for (int i=0; i<10;i++) {
            this->ledCtrl.setLed(0, this->addr[b][i][0], this->addr[b][i][1], true);
            delay(500);
            this->ledCtrl.setLed(0, this->addr[b][i][0], this->addr[b][i][1], false);
        }
    }
}

void BarGraph::setMode(BarGraphMode mode) {
    this->mode = mode;
}

void BarGraph::refresh()
{
    if (this->mode == fuel) {
        this->bar[0] = this->telemetry->stageFuel;
        this->bar[1] = this->telemetry->stageOx;
        this->bar[2] = this->telemetry->stageMonoprop;
        this->bar[3] = this->telemetry->stageElec;
        this->bar[4] = this->telemetry->stageXenon;
    } else {
        this->bar[0] = this->telemetry->stageO2;
        this->bar[1] = this->telemetry->stageH2O;
        this->bar[2] = this->telemetry->stageFood;
        this->bar[3] = this->telemetry->stageCO2;
        this->bar[4] = this->telemetry->stageWaste;
    }

    for (unsigned int b=0; b<5; b++) {
        for (int i=0; i<10;i++) {
            this->ledCtrl.setLed(0, this->addr[b][i][0], this->addr[b][i][1], i < this->bar[b]);
        }
    }
}