#pragma once

#include <Arduino.h>

class Controller;

typedef void (Controller::*Handler)(byte*);

class Controller {
    private:
        Handler handlers[256];
    public:
        char debug_str[80];
        bool connected;
        
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

        Controller();
        void handle_command(byte command, byte* value);
        void handle_hello(byte* value);  // HELLO
        void handle_flags1(byte* value);  // FLAGS1
        void handle_periapsis(byte* value);
};
