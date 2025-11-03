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

#include <LiquidCrystal.h>
#include "Buzzer.hpp"
#include "Clock.hpp"
#include "Joystick.hpp"

/* ---------------------------- Components Usage --------------------------- */

#define VRY_PIN A0
#define VRX_PIN A1
#define BUTTON_PIN 2

#define BUZZER_PIN 3
#define MELODY_SIZE(array) (sizeof (array) / sizeof(Buzzer::note_t))

const Buzzer::note_t melody[] = {
    Buzzer::NOTE_A5, Buzzer::NO_TONE,
    Buzzer::NOTE_A5, Buzzer::NO_TONE,
    Buzzer::NOTE_A5, Buzzer::NO_TONE,
    Buzzer::NO_TONE, Buzzer::NO_TONE
};

#define D4_PIN 4
#define D5_PIN 5
#define D6_PIN 6
#define D7_PIN 7
#define RS_PIN 8
#define EN_PIN 9
#define LCD_ROWS 2
#define LCD_COLS 16

Joystick joystick(VRY_PIN, VRX_PIN, BUTTON_PIN);
LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

/* -------------------------------- Helpers -------------------------------- */

void write_to_lcd(String& buffer) {
    for(int i = 0; i < buffer.length(); i++) {
        if(i % LCD_COLS == 0) {
            lcd.setCursor(0, i / LCD_COLS % LCD_ROWS);
        }

        lcd.print(buffer[i]);
    }
}

void joystick_increment(unsigned long& counter, unsigned long mod) {
    if(joystick.moved_from_to(Joystick::CENTER, Joystick::UP)) {
        counter++;
    
        if(counter == mod) {
            counter = 0;
        }
    } else if(joystick.moved_from_to(Joystick::CENTER, Joystick::DOWN)) {
        if(counter) {
            counter--;
        } else {
            counter = mod - 1;
        }
    } else if(joystick.moved_from_to(Joystick::CENTER, Joystick::RIGHT)) {
        counter += 10;
    
        if(counter >= mod) {
            counter = counter - mod;
        }
    } else if(joystick.moved_from_to(Joystick::CENTER, Joystick::LEFT)) {
        if(counter >= 10) {
            counter -= 10;
        } else {
            counter = mod + counter - 10;
        }
    }
}

void string_padding(String& string, uint8_t padding) {
    while(padding != 0) {
        string += ' ';
        padding--;
    }
}

/* ---------------------------- FSM Declarations --------------------------- */

typedef enum {
    CLOCK,
    ADJUST_CLOCK_SECONDS,
    ADJUST_CLOCK_MINUTES,
    ADJUST_CLOCK_HOURS,
    ALARM,
    ADJUST_ALARM_SECONDS,
    ADJUST_ALARM_MINUTES,
    ADJUST_ALARM_HOURS,
    ALARM_TRIGGERED
} state_t;

typedef void (*fsm_action)();

/* ------------------------------ FSM Context ------------------------------ */

state_t last_state = CLOCK;
state_t current_state = CLOCK;
fsm_action current_action;

String lcd_content;
String lcd_buffer;
String time_buffer;

unsigned long seconds = 0;
unsigned long minutes = 0;
unsigned long hours = 0;

/* ------------------------------ FSM Actions ------------------------------ */

void clock() {
    unsigned long time = Clock::get_time();
    Clock::to_string(time, time_buffer);

    lcd_buffer = "";
    string_padding(lcd_buffer, (LCD_COLS-4)/2);
    lcd_buffer += "TIME";
    string_padding(lcd_buffer, (LCD_COLS-4)/2 + (LCD_COLS-4)%2);
    string_padding(lcd_buffer, (LCD_COLS-8)/2);
    lcd_buffer += time_buffer;
    string_padding(lcd_buffer, (LCD_COLS-8)/2 + (LCD_COLS-8)%2);
}

void adjust_clock_seconds() {
    joystick_increment(seconds, 60);
    Clock::to_string(hours, minutes, seconds, time_buffer);

    lcd_buffer = "";
    string_padding(lcd_buffer, (LCD_COLS-11)/2);
    lcd_buffer += "TIME ADJUST";
    string_padding(lcd_buffer, (LCD_COLS-11)/2 + (LCD_COLS-11)%2);
    string_padding(lcd_buffer, (LCD_COLS-8)/2);
    lcd_buffer += time_buffer;
    string_padding(lcd_buffer, (LCD_COLS-8)/2 + (LCD_COLS-8)%2);
}

void adjust_clock_minutes() {
    joystick_increment(minutes, 60);
    Clock::to_string(hours, minutes, seconds, time_buffer);

    lcd_buffer = "";
    string_padding(lcd_buffer, (LCD_COLS-11)/2);
    lcd_buffer += "TIME ADJUST";
    string_padding(lcd_buffer, (LCD_COLS-11)/2 + (LCD_COLS-11)%2);
    string_padding(lcd_buffer, (LCD_COLS-8)/2);
    lcd_buffer += time_buffer;
    string_padding(lcd_buffer, (LCD_COLS-8)/2 + (LCD_COLS-8)%2);
}

void adjust_clock_hours() {
    joystick_increment(hours, 24);
    Clock::to_string(hours, minutes, seconds, time_buffer);

    lcd_buffer = "";
    string_padding(lcd_buffer, (LCD_COLS-11)/2);
    lcd_buffer += "TIME ADJUST";
    string_padding(lcd_buffer, (LCD_COLS-11)/2 + (LCD_COLS-11)%2);
    string_padding(lcd_buffer, (LCD_COLS-8)/2);
    lcd_buffer += time_buffer;
    string_padding(lcd_buffer, (LCD_COLS-8)/2 + (LCD_COLS-8)%2);
}

void alarm() {
    unsigned long time = Clock::get_alarm();
    
    if(time != CLOCK_INVALID_TIME) {
        Clock::to_string(time, time_buffer);
    } else {
        time_buffer = "DISABLED";
    }

    lcd_buffer = "";
    string_padding(lcd_buffer, (LCD_COLS-5)/2);
    lcd_buffer += "ALARM";
    string_padding(lcd_buffer, (LCD_COLS-5)/2 + (LCD_COLS-5)%2);
    string_padding(lcd_buffer, (LCD_COLS-8)/2);
    lcd_buffer += time_buffer;
    string_padding(lcd_buffer, (LCD_COLS-8)/2 + (LCD_COLS-8)%2);
}

void adjust_alarm_seconds() {
    joystick_increment(seconds, 60);
    Clock::to_string(hours, minutes, seconds, time_buffer);

    lcd_buffer = "";
    string_padding(lcd_buffer, (LCD_COLS-12)/2);
    lcd_buffer += "ALARM ADJUST";
    string_padding(lcd_buffer, (LCD_COLS-12)/2 + (LCD_COLS-12)%2);
    string_padding(lcd_buffer, (LCD_COLS-8)/2);
    lcd_buffer += time_buffer;
    string_padding(lcd_buffer, (LCD_COLS-8)/2 + (LCD_COLS-8)%2);
}

void adjust_alarm_minutes() {
    joystick_increment(minutes, 60);
    Clock::to_string(hours, minutes, seconds, time_buffer);

    lcd_buffer = "";
    string_padding(lcd_buffer, (LCD_COLS-12)/2);
    lcd_buffer += "ALARM ADJUST";
    string_padding(lcd_buffer, (LCD_COLS-12)/2 + (LCD_COLS-12)%2);
    string_padding(lcd_buffer, (LCD_COLS-8)/2);
    lcd_buffer += time_buffer;
    string_padding(lcd_buffer, (LCD_COLS-8)/2 + (LCD_COLS-8)%2);
}

void adjust_alarm_hours() {
    joystick_increment(hours, 24);
    Clock::to_string(hours, minutes, seconds, time_buffer);

    lcd_buffer = "";
    string_padding(lcd_buffer, (LCD_COLS-12)/2);
    lcd_buffer += "ALARM ADJUST";
    string_padding(lcd_buffer, (LCD_COLS-12)/2 + (LCD_COLS-12)%2);
    string_padding(lcd_buffer, (LCD_COLS-8)/2);
    lcd_buffer += time_buffer;
    string_padding(lcd_buffer, (LCD_COLS-8)/2 + (LCD_COLS-8)%2);
}

void alarm_triggered() {
    unsigned long time = Clock::get_alarm();
    
    if(time != CLOCK_INVALID_TIME) {
        Clock::to_string(time, time_buffer);
    } else {
        time_buffer = "DISABLED";
    }

    lcd_buffer = "";
    string_padding(lcd_buffer, (LCD_COLS-13)/2);
    lcd_buffer += "ALARM RINGING";
    string_padding(lcd_buffer, (LCD_COLS-13)/2 + (LCD_COLS-13)%2);
    string_padding(lcd_buffer, (LCD_COLS-8)/2);
    lcd_buffer += time_buffer;
    string_padding(lcd_buffer, (LCD_COLS-8)/2 + (LCD_COLS-8)%2);
}

/* --------------------------------- Setup --------------------------------- */

void setup() {
    Serial.begin(9600);

    Buzzer::begin(BUZZER_PIN);

    Clock::begin();
    
    lcd.begin(LCD_COLS, LCD_ROWS);
    lcd.setCursor(0, 0);
    lcd.noCursor();

    lcd_content.reserve(LCD_ROWS * LCD_COLS);
    lcd_buffer.reserve(LCD_ROWS * LCD_COLS);
    time_buffer.reserve(8);
}

/* ---------------------------------- Loop --------------------------------- */

void loop() {
    joystick.update();

    switch(current_state) {
    case CLOCK:
        current_action = clock;

        if(joystick.is_rising_edge()) {
            last_state = current_state;
            current_state = ADJUST_CLOCK_SECONDS;
            seconds = 0;
            minutes = 0;
            hours = 0;
        } else if(joystick.moved_from_to(Joystick::CENTER, Joystick::UP)) {
            last_state = current_state;
            current_state = ALARM;
        } else if(Clock::is_alarm_triggered()) {
            last_state = current_state;
            current_state = ALARM_TRIGGERED;

            Buzzer::play(melody, MELODY_SIZE(melody));
        }

        break;
    case ADJUST_CLOCK_SECONDS:
        current_action = adjust_clock_seconds;

        if(joystick.is_rising_edge()) {
            last_state = current_state;
            current_state = ADJUST_CLOCK_MINUTES;
        }

        break;
    case ADJUST_CLOCK_MINUTES:
        current_action = adjust_clock_minutes;

        if(joystick.is_rising_edge()) {
            last_state = current_state;
            current_state = ADJUST_CLOCK_HOURS;
        }

        break;
    case ADJUST_CLOCK_HOURS:
        current_action = adjust_clock_hours;

        if(joystick.is_rising_edge()) {
            last_state = current_state;
            current_state = CLOCK;

            Clock::set_time(hours, minutes, seconds);
        }

        break;
    case ALARM:
        current_action = alarm;

        if(joystick.is_rising_edge()) {
            last_state = current_state;
            current_state = ADJUST_ALARM_SECONDS;
            seconds = 0;
            minutes = 0;
            hours = 0;
        }  else if(joystick.moved_from_to(Joystick::CENTER, Joystick::DOWN)) {
            last_state = current_state;
            current_state = CLOCK;
        }  else if(Clock::is_alarm_triggered()) {
            last_state = current_state;
            current_state = ALARM_TRIGGERED;

            Buzzer::play(melody, MELODY_SIZE(melody));
        }

        break;
    case ADJUST_ALARM_SECONDS:
        current_action = adjust_alarm_seconds;

        if(joystick.is_rising_edge()) {
            last_state = current_state;
            current_state = ADJUST_ALARM_MINUTES;
        }

        break;
    case ADJUST_ALARM_MINUTES:
        current_action = adjust_alarm_minutes;

        if(joystick.is_rising_edge()) {
            last_state = current_state;
            current_state = ADJUST_ALARM_HOURS;
        }

        break;
    case ADJUST_ALARM_HOURS:
        current_action = adjust_alarm_hours;

        if(joystick.is_rising_edge()) {
            last_state = current_state;
            current_state = ALARM;

            Clock::set_alarm(hours, minutes, seconds);
        }

        break;
    case ALARM_TRIGGERED:
        current_action = alarm_triggered;

        if(joystick.is_rising_edge()) {
            current_state = last_state;
            last_state = ALARM_TRIGGERED;
            
            Clock::disable_alarm();
            Buzzer::stop();
        }

        break;
    }

    current_action();

    // Update LCD if necessary
    if(lcd_content != lcd_buffer) {
        write_to_lcd(lcd_buffer);
        lcd_content = lcd_buffer;
    }
}