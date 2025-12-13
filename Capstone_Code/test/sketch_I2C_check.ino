/*
#include <TinyWireM.h> // Use TinyWireM for ATtiny I2C

#define LED_PIN 5  // PA5

void setup() {
  TinyWireM.begin();      // Initialize I2C
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  for (byte address = 1; address < 127; address++) {
    TinyWireM.beginTransmission(address);
    byte error = TinyWireM.endTransmission();
    if (error == 0) {
      // Device found at this address
      // Blink LED to indicate address
      for (byte i = 0; i <= (address & 0x0F); i++) {  // Blink lower 4 bits
        digitalWrite(LED_PIN, HIGH);
        delay(200);
        digitalWrite(LED_PIN, LOW);
        delay(200);
      }
      delay(1000); // Pause before next address
      for (byte i = 0; i <= (address & 0xF0)/(2^4); i++) {  // Blink upper 4 bits
        digitalWrite(LED_PIN, HIGH);
        delay(200);
        digitalWrite(LED_PIN, LOW);
        delay(200);
      }

    }
  }
  delay(3000); // Pause before full scan repeats
}
*/
#include <TinyWireM.h>  // Make sure this library is installed

#define I2C_ADDR 0x3C    // 7-bit I2C address
#define LED_PIN 5        // PA5

void setup() {
  TinyWireM.begin();
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  TinyWireM.beginTransmission(I2C_ADDR);
  byte err = TinyWireM.endTransmission();
  
  if (err == 0) {
    // Device acknowledged, blink LED fast
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  } else {
    // No response, blink LED slow
    digitalWrite(LED_PIN, HIGH);
    delay(3000);
    digitalWrite(LED_PIN, LOW);
    delay(3000);
  }
}
