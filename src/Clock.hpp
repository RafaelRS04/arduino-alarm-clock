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

#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <Arduino.h>
#include "timers.hpp"

#define SECONDS_PER_DAY 86400UL
#define CLOCK_INVALID_TIME SECONDS_PER_DAY

namespace Clock {
    void begin();

    unsigned long time_to_seconds(
        unsigned long hours,
        unsigned long minutes,
        unsigned long seconds
    );

    void set_time(
        unsigned long hours,
        unsigned long minutes,
        unsigned long seconds
    );

    unsigned long get_time();
    void pause();
    void resume();
    
    void set_alarm(
        unsigned long hours,
        unsigned long minutes,
        unsigned long seconds
    );

    unsigned long get_alarm();
    void enable_alarm();
    void disable_alarm();
    boolean is_alarm_triggered();

    // Without validation
    void to_string(
        unsigned long hours,
        unsigned long minutes,
        unsigned long seconds,
        String& output
    );

    void to_string(unsigned long time_seconds, String& output);
};

#endif /* Clock.hpp */