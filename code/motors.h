#ifndef MOTORS_H
#define MOTORS_H

#include "Arduino.h"

// Motor Pin Indeces
#define MOTORSR_F A3
#define MOTORSR_B A4
#define MOTORSL_F A1
#define MOTORSL_B A2
#define MOTORSV_F 0
#define MOTORSV_B 1

// Accessory pin indeces
#define SWITCH1_PIN 2
#define SWITCH2_PIN 7

// Button Indeces in the packet
#define LEFT_ANALOG_Y_INDEX 0
#define RIGHT_ANALOG_Y_INDEX 1
#define L2_INDEX 6
#define R2_INDEX 7
#define BUTTON1_INDEX 2
#define BUTTON2_INDEX 3
#define BUTTON3_INDEX 4
#define BUTTON4_INDEX 5

#define DEADZONE 25

// Motor Class
class Motor {
    private:
        int f_pin;
        int b_pin;
    public:
        Motor(int f, int b) {
            this->f_pin = f;
            this->b_pin = b;
        }

        void init();
        
        void forward();

        void backward();

        void stop();
};

#endif
