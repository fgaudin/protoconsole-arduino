#pragma once

#include <Arduino.h>
#include <Telemetry.h>
#include <MainDisplay.h>


class Controller;

typedef void (Controller::*Handler)(byte*);

class Controller {
    private:
        Handler handlers[256];
        Telemetry telemetry;
        MainDisplay display;
    public:
        bool connected;
        
        Controller();
        void init();
        void update();
        void handle_command(byte command, byte* value);
        void handle_hello(byte* value);  // HELLO
        void handle_flags1(byte* value);  // FLAGS1
        void handle_periapsis(byte* value);
};
