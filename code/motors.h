#ifndef MOTORS_H
#define MOTORS_H

#include "Arduino.h"

// Motor Pin Indeces
#define MOTOR1_F A0
#define MOTOR1_B A1
#define MOTOR2_F A2
#define MOTOR2_B A3
#define MOTOR3_F A4
#define MOTOR3_B A5
#define MOTOR4_F A6
#define MOTOR4_B A7

#define FRONT_VERTICAL_F 3
#define FRONT_VERTICAL_B 5
#define REAR_VERTICAL_F 6
#define REAR_VERTICAL_B 9

// Packet Indeces
#define UP_BUTTON_INDEX 7
#define DOWN_BUTTON_INDEX 6

#define LEFT_ANALOG_X_INDEX 0
#define LEFT_ANALOG_Y_INDEX 1
#define RIGHT_ANALOG_Y_INDEX 2
#define RIGHT_ANALOG_X_INDEX 3

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

        void forward();

        void backward();

        void stop();
};

#endif
