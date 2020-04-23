#pragma once

#include <Arduino.h>

class Telemetry {
    public:
        bool solar_panel;
        bool gear;
        bool docked;
        bool lights;
        bool sas;
        bool rcs;
        bool brake;
        bool engine;
        bool antenna;

        long periapsis;
        long apoapsis;
        long surface_velocity;
        long orbital_velocity;

        Telemetry();
};
