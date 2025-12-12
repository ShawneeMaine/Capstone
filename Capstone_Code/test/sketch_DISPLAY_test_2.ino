#include <TinyWireM.h>  // ATtiny I²C library

#define I2C_ADDR 0x3C   // 7-bit OLED I²C address
#define LED_PIN 5       // PA5

// Minimal 5x7 font for B and E
const uint8_t font_B[] = {0x7F, 0x49, 0x49, 0x49, 0x36};
const uint8_t font_E[] = {0x7F, 0x49, 0x49, 0x49, 0x41};

#define NUM_CHARS 11     // Display 11 letters

// --- I²C helpers ---
void sendCommand(uint8_t cmd) {
  TinyWireM.beginTransmission(I2C_ADDR);
  TinyWireM.send(0x00);  // Co = 0, D/C# = 0 → command
  TinyWireM.send(cmd);
  TinyWireM.endTransmission();
}

void sendData(uint8_t data) {
  TinyWireM.beginTransmission(I2C_ADDR);
  TinyWireM.send(0x40);  // Co = 0, D/C# = 1 → data
  TinyWireM.send(data);
  TinyWireM.endTransmission();
}

// Set cursor to top-left
void setCursorTopLeft() {
  sendCommand(0x00); // lower column address
  sendCommand(0x10); // higher column address
  sendCommand(0xB0); // page 0
}

// Clear display (128x32)
void clearDisplay() {
  setCursorTopLeft();
  for (uint16_t i = 0; i < 128 * 4; i++) { // 128 columns × 4 pages for 32-row display
    sendData(0x00);
  }
  setCursorTopLeft(); // reset cursor to top-left after clearing
}

// Write a single letter to the display
void writeLetter(const uint8_t *letter) {
  for (uint8_t i = 0; i < 5; i++) sendData(letter[i]);
  sendData(0x00); // spacing
}

// Write a full message of NUM_CHARS letters
void writeMessage(const uint8_t *letter) {
  clearDisplay();
  setCursorTopLeft();
  for (uint8_t i = 0; i < NUM_CHARS; i++) {
    writeLetter(letter);
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);

  // Startup delay for removing programmer wires
  for (int i = 0; i < 20; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(500);
  }

  TinyWireM.begin();

  // Initialize OLED (128x32)
  sendCommand(0xAE); sendCommand(0xD5); sendCommand(0x80);
  sendCommand(0xA8); sendCommand(0x1F); sendCommand(0xD3);
  sendCommand(0x00); sendCommand(0x40); sendCommand(0x8D);
  sendCommand(0x14); sendCommand(0x20); sendCommand(0x00);
  sendCommand(0xA1); sendCommand(0xC8); sendCommand(0xDA);
  sendCommand(0x02); sendCommand(0x81); sendCommand(0x8F);
  sendCommand(0xD9); sendCommand(0xF1); sendCommand(0xDB);
  sendCommand(0x40); sendCommand(0xA4); sendCommand(0xA6);
  sendCommand(0xAF);

  clearDisplay();
}

void loop() {
  // --- I²C check ---
  TinyWireM.beginTransmission(I2C_ADDR);
  byte err = TinyWireM.endTransmission();
  digitalWrite(LED_PIN, err == 0 ? HIGH : LOW); // simple on/off LED, no delay

  // Display 11 Bs for 1 second
  writeMessage(font_B);
  delay(1000);

  // Display 11 Es for 1 second
  writeMessage(font_E);
  delay(1000);
}
