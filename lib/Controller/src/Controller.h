#pragma once

#include <Arduino.h>
#include <Telemetry.h>
#include <MainDisplay.h>


class Controller;

typedef void (Controller::*Handler)(byte*);

class Controller {
    private:
        unsigned long lastUpdate;
        Handler handlers[256];
        Telemetry telemetry;
        MainDisplay display;

    public:
        bool connected;
        
        Controller();
        void init();
        void checkInputs();
        void update();
        void handle_command(byte command, byte* value);
        void handle_hello(byte* value);
        void handle_flags1(byte* value);
        void handle_pitch(byte* value);
        void handle_twr(byte* value);
        void handle_periapsis(byte* value);
        void handle_apoapsis(byte* value);
        void handle_vertical_speed(byte* value);
        void handle_horizontal_speed(byte* value);
        void handle_altitude(byte* value);
};
