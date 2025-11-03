#include "Buzzer.hpp"

#define BUZZER_PIN 3
#define MELODY_SIZE(array) (sizeof (array) / sizeof(Buzzer::note_t))

Buzzer::note_t melody[] = {
    Buzzer::NOTE_C4, Buzzer::NOTE_C4, Buzzer::NOTE_D4, Buzzer::NOTE_C4,
    Buzzer::NOTE_F4, Buzzer::NOTE_E4, Buzzer::NOTE_C4, Buzzer::NOTE_C4,
    Buzzer::NOTE_D4, Buzzer::NOTE_C4, Buzzer::NOTE_G4, Buzzer::NOTE_F4,
    Buzzer::NOTE_C4, Buzzer::NOTE_C4, Buzzer::NOTE_C5, Buzzer::NOTE_A4,
    Buzzer::NOTE_F4, Buzzer::NOTE_E4, Buzzer::NOTE_D4, Buzzer::NOTE_AS4,
    Buzzer::NOTE_AS4, Buzzer::NOTE_A4, Buzzer::NOTE_F4, Buzzer::NOTE_G4,
    Buzzer::NOTE_F4, Buzzer::NO_TONE, Buzzer::NO_TONE, Buzzer::NO_TONE
};

void setup() {
    Serial.begin(9600);
    Buzzer::begin(BUZZER_PIN);
}
    
void loop() {
    // Play test
    Buzzer::play(melody, MELODY_SIZE(melody));
    Serial.println("Play");
    
    delay(5000);
    
    // Pause test
    Buzzer::pause();
    Serial.println("Pause");
    
    delay(5000);

    // Resume test
    Buzzer::resume();
    Serial.println("Resume");

    delay(5000);

    // Stop test (resume don't have effect)
    Buzzer::stop();
    Serial.println("Stop");
    Buzzer::resume();

    delay(5000);
}

