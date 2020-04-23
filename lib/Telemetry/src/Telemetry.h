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

        byte twr;
        byte pitch;

        long altitude;
        long periapsis;
        long apoapsis;
        long verticalVelocity;
        long horizontalVelocity;

        Telemetry();
};
