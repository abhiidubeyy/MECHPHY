#include <Arduino.h>

#if !defined(ARDUINO_ESP32C3_DEV)
#define DISABLE_CODE_FOR_RECEIVER 
#endif

#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp> 

const int BUTTON_PIN = 0; // D3 on ESP8266

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Enable internal pull-up resistor

  Serial.begin(115200);
  while (!Serial);

  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
  Serial.print(F("Send IR signals at pin "));
  Serial.println(IR_SEND_PIN);

  IrSender.begin(); 
  disableLEDFeedback(); 
}

void loop() {
  // Check if the button is pressed (active LOW)
  if (digitalRead(BUTTON_PIN) == LOW) {
    // Define Soldier ID, Weapon ID, and Firing Mode
    int soldierID = 12;  // 2-digit Soldier ID
    int weaponID = 5;    // 1-digit Weapon ID
    int firingMode = 2;  // 1-digit Firing Mode

    // Combine the values into a 4-digit decimal number (4581 in this case)
    int combinedValue = soldierID * 100 + weaponID * 10 + firingMode;

    // Convert the combined value into hex
    uint32_t hexCode = combinedValue;

    // Send the hex code via IR
    Serial.print("Sending IR signal (raw data): 0x");
    Serial.println(hexCode, HEX);

    IrSender.sendNECRaw(hexCode, 0); // Send the code once (no repeats)

    delay(500); // Debounce delay to avoid multiple triggers
  }
}
