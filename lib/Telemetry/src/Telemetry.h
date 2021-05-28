#pragma once

#include <Arduino.h>

enum Mode {debug, ascent, orbit, descent, docking};

class Telemetry {
    public:
        bool solarPanel;
        bool gear;
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

        unsigned long met;

        float altitude;
        float periapsis;
        float apoapsis;
        float verticalSpeed;
        float orbitalSpeed;
        
        int32_t apoapsisTime;
        int32_t periapsisTime;
        float timeToNextManeuver;
        
        int16_t pitch;

        char q[6] = "#";
        char twr[6] = "#";

        Telemetry();
};
