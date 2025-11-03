#include "Joystick.hpp"

#define VRY_PIN A0
#define VRX_PIN A1
#define BUTTON_PIN 2

Joystick joystick(VRY_PIN, VRX_PIN, BUTTON_PIN);

void setup() {
    Serial.begin(9600);
}

void loop() {
    joystick.update();

    if(joystick.is_rising_edge()) {
        Serial.println("Rising Edge");
    }

    if(joystick.is_falling_edge()) {
        Serial.println("Falling Edge");
    }

    if(joystick.moved_from_to(Joystick::CENTER, Joystick::RIGHT)) {
        Serial.println("Moved from center to right");

        Joystick::Vector vector = joystick.get_analog_read();
        Serial.print("X: ");
        Serial.println(vector.x);
        Serial.print("Y: ");
        Serial.println(vector.y);
    }

    if(joystick.is_pressed()) {
        Joystick::direction_t direction = joystick.get_analog_direction();

        if(direction == Joystick::CENTER)
            Serial.println("Pressed and on center");

        delay(1000);
    }
}