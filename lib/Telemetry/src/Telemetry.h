#pragma once

#include <Arduino.h>

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
        bool antenna;

        float twr;
        int pitch;
        int stageFuel;

        long altitude;
        long periapsis;
        long apoapsis;
        long verticalSpeed;
        long horizontalSpeed;

        Telemetry();
};
