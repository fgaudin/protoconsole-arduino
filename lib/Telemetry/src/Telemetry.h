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

        long met;

        char altitude[6] = "#";
        char periapsis[6] = "#";
        char apoapsis[6] = "#";
        char verticalSpeed[6] = "#";
        char horizontalSpeed[6] = "#";
        char pitch[6] = "#";
        char q[6] = "#";
        char twr[6] = "#";

        Telemetry();
        void update(char id, byte* data);
};
