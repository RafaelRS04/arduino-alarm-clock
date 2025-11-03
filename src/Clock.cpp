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

#include "Clock.hpp"
#include "debug.hpp"

using namespace Clock;

/* ----------------------------- Clock context ----------------------------- */
static volatile unsigned long current_seconds = 0;

/* ----------------------------- Alarm context ----------------------------- */
static volatile unsigned long alarm_seconds = CLOCK_INVALID_TIME;
static volatile boolean alarm_enabled = false;
static volatile boolean alarm_triggered = false;

/* ----------------------------- Clock handler ----------------------------- */
void clock_handler() {
    current_seconds++;

    if(current_seconds >= SECONDS_PER_DAY) {
        current_seconds = 0;
    }

    if(alarm_enabled && current_seconds == alarm_seconds) {
        alarm_enabled = false;
        alarm_triggered = true;
    }
}

/* ------------------------ Clock public interface ------------------------- */

void Clock::begin() {
    ITimer4.init();

    if(ITimer4.setFrequency(1.0f, clock_handler)) {
        LOG("SERIAL LOG: clock service is up [ITimer4]");
    } else {
        LOG("SERIAL ERROR: clock service setup error [ITimer4]");
    }
}

unsigned long Clock::time_to_seconds(
    unsigned long hours,
    unsigned long minutes,
    unsigned long seconds
) {
    unsigned long time_seconds = hours * 3600 + minutes * 60 + seconds;
    
    if(time_seconds >= SECONDS_PER_DAY) {
        LOG("SERIAL ERROR: invalid time passed to time_to_seconds()");
        return CLOCK_INVALID_TIME;
    }

    return time_seconds;
}

void Clock::set_time(
    unsigned long hours,
    unsigned long minutes,
    unsigned long seconds
) {
    unsigned long new_time = time_to_seconds(hours, minutes, seconds);

    if(new_time == CLOCK_INVALID_TIME) {
        LOG("SERIAL ERROR: invalid time passed to set_time()");
        return;
    }

    ITimer4.pauseTimer();

    current_seconds = new_time;

    ITimer4.resumeTimer();
}

unsigned long Clock::get_time() {
    return current_seconds;
}

void Clock::pause() {
    ITimer4.pauseTimer();
}

void Clock::resume() {
    ITimer4.resumeTimer();
}

void Clock::set_alarm(
    unsigned long hours,
    unsigned long minutes,
    unsigned long seconds
) {
    unsigned long new_alarm_time = time_to_seconds(hours, minutes, seconds);

    if(new_alarm_time == CLOCK_INVALID_TIME) {
        LOG("SERIAL ERROR: invalid time passed to set_alarm()");
        return;
    }

    disable_alarm();

    alarm_seconds = new_alarm_time;
    
    enable_alarm();
}

unsigned long Clock::get_alarm() {
    if(alarm_enabled || alarm_triggered)
        return alarm_seconds;

    return CLOCK_INVALID_TIME;
}

void Clock::enable_alarm() {
    alarm_triggered = false;
    alarm_enabled = true;
}

void Clock::disable_alarm() {
    alarm_triggered = false;
    alarm_enabled = false;
}

boolean Clock::is_alarm_triggered() {
    return alarm_triggered;
}

void Clock::to_string(
    unsigned long hours,
    unsigned long minutes,
    unsigned long seconds,
    String& output
) {
    output = ""; 

    if (hours < 10) output += '0';
    output += hours;
    output += ':';
    if (minutes < 10) output += '0';
    output += minutes;
    output += ':';
    if (seconds < 10) output += '0';
    output += seconds;
}


void Clock::to_string(unsigned long time_seconds, String& output) {
    if(time_seconds >= SECONDS_PER_DAY) {
        LOG("SERIAL ERROR: invalid time passed to set_alarm()");
        return;
    }

    unsigned int hours = (time_seconds / 3600);
    unsigned int minutes = (time_seconds % 3600) / 60;
    unsigned int seconds = time_seconds % 60;

    to_string(hours, minutes, seconds, output);
}