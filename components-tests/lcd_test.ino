#include <LiquidCrystal.h>

#define D4_PIN 4
#define D5_PIN 5
#define D6_PIN 6
#define D7_PIN 7
#define RS_PIN 8
#define EN_PIN 9

LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

void setup() {
    Serial.begin(9600);
    lcd.begin(16, 2);

    // Print test
    lcd.setCursor(0, 0);
    lcd.print("Hello World!");
    lcd.setCursor(0, 1);
    lcd.print("Super Test");
}

void loop() {
}
