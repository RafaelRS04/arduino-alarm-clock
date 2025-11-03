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

#include "Buzzer.hpp"
#include "debug.hpp"

using namespace Buzzer;

/* ---------------------------- Buzzer context ----------------------------- */
static const note_t *current_notes = NULL;
static uint16_t current_size = 0;
static volatile uint16_t index = 0;
static uint8_t buzzer_pin = 0;

/* ---------------------------- Buzzer handler ----------------------------- */
void buzzer_handler() {
    if(current_notes) {

        noTone(buzzer_pin);

        if(index == current_size) {
            index = 0;
        }

        note_t note = current_notes[index];

        if(note != NO_TONE) {
            tone(buzzer_pin, note);
        }

        index++;
    }
}

/* ------------------------ Buzzer public interface ------------------------ */
void Buzzer::begin(uint8_t pwm_pin, uint16_t bpm=DEFAULT_BUZZER_BPM) {
    float frequency = bpm / 60;
    
    buzzer_pin = pwm_pin;
    pinMode(pwm_pin, OUTPUT);

    ITimer3.init();

    if(ITimer3.setFrequency(frequency, buzzer_handler)) {
        LOG("SERIAL LOG: buzzer service is up [ITimer3]");
        ITimer3.pauseTimer();
    } else {
        LOG("SERIAL ERROR: buzzer service setup error [ITimer3]");
    }
}

void Buzzer::play(const note_t *notes, uint16_t size) {
    if(notes == NULL) {
        LOG("SERIAL ERROR: NULL pointer passed to play() function");
        return;
    }

    if(size == 0) {
        LOG("SERIAL ERROR: zero size passed to play() function");
        return;
    }

    ITimer3.pauseTimer();

    current_notes = notes;
    current_size = size;
    index = 0;

    ITimer3.resumeTimer();
}

void Buzzer::pause() {
    ITimer3.pauseTimer();
    noTone(buzzer_pin);
}

void Buzzer::resume() {
    if(current_notes == NULL) {
        LOG("SERIAL ERROR: invalid call to resume() function, current notes points to NULL");
        return; 
    }
    
    ITimer3.resumeTimer();
}

void Buzzer::stop() {
    ITimer3.pauseTimer();
    noTone(buzzer_pin);

    current_notes = NULL;
    current_size = 0;
    index = 0;
}