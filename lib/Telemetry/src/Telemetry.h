#pragma once

#include <Arduino.h>

class Telemetry {
    public:
        int solarPanel;
        int gear;
        bool docked;
        bool lights;
        bool sas;
        bool rcs;
        bool brake;
        bool engine;
        int antenna;
        bool dot05g;
        bool contact;

        float twr;
        int pitch;
        int stageFuel;
        int stageOx;
        int stageMonoprop;
        int stageElec;
        int stageXenon;
        int stageO2;
        int stageH2O;
        int stageFood;
        int stageCO2;
        int stageWaste;

        long altitude;
        long periapsis;
        long apoapsis;
        long verticalSpeed;
        long horizontalSpeed;

        Telemetry();
};
