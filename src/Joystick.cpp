/***********************************************************************************

    MIT License

    Copyright (c) 2025 Rafael Rodrigues Sanches

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

***********************************************************************************/

#include "Joystick.hpp"

typedef Joystick::direction_t direction_t;
typedef Joystick::Vector Vector;

/* ----------------------- Joystick public interface ----------------------- */

Joystick::Joystick(uint8_t analogx_pin, uint8_t analogy_pin, uint8_t button_pin) {
    pinMode(button_pin, INPUT_PULLUP);

    this->analogx_pin = analogx_pin;
    this->analogy_pin = analogy_pin;
    this->button_pin = button_pin;

    debounce_delay = DEFAULT_JOYSTICK_DEBOUNCE_DELAY;

    analog_correction = {
        .x = DEFAULT_ANALOGX_CORRECTION,
        .y = DEFAULT_ANALOGY_CORRECTION
    };

    activation_percent = DEFAULT_JOYSTICK_ACTIVATION_PERCENT;

    analog_state = CENTER;
    analog_poll = CENTER;

    button_state = LOW;
    button_poll = LOW;

    analog_debounce_time = millis();
    button_debounce_time = millis();
}

void Joystick::update() {
    const int16_t max_travel = (JOYSTICK_ANALOG_READ_MAX - JOYSTICK_ANALOG_READ_MIN) / 2;
    int16_t activation_threshold = activation_percent * max_travel;

    Vector analog = get_analog_read();

    direction_t analog_current_poll = CENTER;
    
    if(analog.x >= activation_threshold) {
        analog_current_poll = RIGHT;
    } else if(analog.x <= -activation_threshold) {
        analog_current_poll = LEFT;
    }

    if(analog.y >= activation_threshold) {
        analog_current_poll = UP;
    } else if(analog.y<= -activation_threshold) {
        analog_current_poll = DOWN;
    }

    if(analog_current_poll != analog_poll) {
        analog_debounce_time = millis();
    }

    analog_poll = analog_current_poll;

    if(millis() - analog_debounce_time > debounce_delay) {
        last_analog_state = analog_state;
        analog_state = analog_poll;
    }

    uint8_t button_current_poll = !digitalRead(button_pin);

    if(button_current_poll != button_poll) {
        button_debounce_time = millis();
    }

    button_poll = button_current_poll;

    if(millis() - button_debounce_time > debounce_delay) {
        last_button_state = button_state;
        button_state = button_poll;
    }
}

Vector Joystick::get_analog_read() {
    Vector current_read {
        .x = analogRead(analogx_pin),
        .y = analogRead(analogy_pin)
    };

    return current_read - analog_correction;
}

direction_t Joystick::get_analog_direction() {
    return analog_state;
}

boolean Joystick::moved_from_to(direction_t from, direction_t to) {
    return from == last_analog_state && to == analog_state;
}

boolean Joystick::is_pressed() {
    return button_state;
}

boolean Joystick::is_rising_edge() {
    return last_button_state == LOW && button_state == HIGH;
}

boolean Joystick::is_falling_edge() {
    return last_button_state == HIGH && button_state == LOW;
}
