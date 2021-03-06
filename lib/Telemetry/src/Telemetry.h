#pragma once

#include <Arduino.h>

enum Mode {debug, ascent, orbit, descent, docking};

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
        bool master_alarm;
        bool stage;

        float twr;
        float q;
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
