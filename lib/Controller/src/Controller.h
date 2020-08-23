#pragma once

#include <Arduino.h>
#include <Telemetry.h>
#include <MainDisplay.h>
#include <BarGraph.h>
#include <LedDisplay.h>


class Controller;

typedef void (Controller::*Handler)(byte*);

class Controller {
    private:
        unsigned long lastUpdate;
        Handler handlers[64];
        Telemetry telemetry;
        MainDisplay display;
        BarGraph bars;
        LedDisplay leds;

    public:
        bool connected;
        
        Controller();
        void init();
        void checkInputs();
        void update();
        void handle_command(byte command, byte* value);
        void handle_hello(byte* value);
        void handle_flags1(byte* value);
        void handle_flags2(byte* value);
        void handle_pitch(byte* value);
        void handle_twr(byte* value);
        void handleStageFuel(byte* value);
        void handleStageOx(byte* value);
        void handleStageMonoprop(byte* value);
        void handleStageElec(byte* value);
        void handleStageXenon(byte* value);
        void handleStageO2(byte* value);
        void handleStageH2O(byte* value);
        void handleStageFood(byte* value);
        void handleStageCO2(byte* value);
        void handleStageWaste(byte* value);
        void handle_periapsis(byte* value);
        void handle_apoapsis(byte* value);
        void handle_vertical_speed(byte* value);
        void handle_horizontal_speed(byte* value);
        void handle_altitude(byte* value);
};
