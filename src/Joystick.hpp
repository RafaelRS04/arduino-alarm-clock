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

#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP

#include <Arduino.h>

#define DEFAULT_JOYSTICK_DEBOUNCE_DELAY 50
#define DEFAULT_JOYSTICK_ACTIVATION_PERCENT 0.95

#define DEFAULT_ANALOGX_CORRECTION 512
#define DEFAULT_ANALOGY_CORRECTION 512

#define JOYSTICK_ANALOG_READ_MIN 0
#define JOYSTICK_ANALOG_READ_MAX 1023

class Joystick {
public:
    typedef enum {
        UP,
        DOWN,
        CENTER,
        LEFT,
        RIGHT
    } direction_t;

    struct Vector {
        int16_t x, y;

        Vector operator+(const Vector& other) {
            Vector sum {
                .x = x + other.x,
                .y = y + other.y
            };

            return sum;
        }

        Vector operator-(const Vector& other) {
            Vector diff {
                .x = x - other.x,
                .y = y - other.y
            };

            return diff;
        }
    };

    Joystick(uint8_t analogx_pin, uint8_t analogy_pin, uint8_t button_pin);
    
    void update();

    Vector get_analog_read();
    direction_t get_analog_direction();
    boolean moved_from_to(direction_t from, direction_t to);

    boolean is_pressed();
    boolean is_rising_edge();
    boolean is_falling_edge();

    Vector analog_correction;
    float activation_percent;
    unsigned long debounce_delay;
private:
    uint8_t analogx_pin;
    uint8_t analogy_pin;
    uint8_t button_pin;

    direction_t last_analog_state;
    direction_t analog_state;
    direction_t analog_poll;

    uint8_t last_button_state;
    uint8_t button_state;
    uint8_t button_poll;

    unsigned long analog_debounce_time;
    unsigned long button_debounce_time;
};

#endif /* Joystick.hpp */