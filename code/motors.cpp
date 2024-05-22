#include "motors.h"

void Motor::forward() {
    digitalWrite(this->f_pin, HIGH);
    digitalWrite(this->b_pin, LOW);
}

void Motor::backward() {
    digitalWrite(this->f_pin, LOW);
    digitalWrite(this->b_pin, HIGH);
}

void Motor::stop() {
    digitalWrite(this->f_pin, LOW);
    digitalWrite(this->b_pin, LOW);
}