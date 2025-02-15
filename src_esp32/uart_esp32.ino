#include <Arduino.h>

const int ledPin = 2; // Define the LED pin
const int buttonPin = 14; // Define the button pin
const unsigned long debounceDelay = 50; // Debounce delay in milliseconds

unsigned long lastDebounceTime = 0; // The last time the button state was toggled
bool lastButtonState = LOW; // The previous state of the button
bool buttonState = LOW; // The current state of the button

void setup() {
  // Initialize serial communication with specified baud rate and parity
  Serial.begin(9600, SERIAL_8O1); // 8 data bits, odd parity, 1 stop bit
  pinMode(ledPin, OUTPUT); // Set the LED pin as an output
  pinMode(buttonPin, INPUT); // Set the button pin as an input with internal pull-up resistor
}

void loop() {
  // Check if data is available to read
  if (Serial.available() > 0) {
    // Read the incoming byte
    char incomingByte = Serial.read();
    // Print the received byte as ASCII character
    Serial.print(incomingByte);
    
    // Check if the incoming byte is 0x55
    if (incomingByte == 0x55) {
      // Toggle the LED on pin 2
      digitalWrite(ledPin, !digitalRead(ledPin));
    }
  }

  // Read the state of the button
  bool reading = digitalRead(buttonPin);

  // Check if the button state has changed
  if (reading != lastButtonState) {
    // Reset the debounce timer
    lastDebounceTime = millis();
  }

  // Check if the debounce delay has passed
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // If the button state has changed
    if (reading != buttonState) {
      buttonState = reading;

      // Only toggle the LED if the button is pressed
      if (buttonState == HIGH) {
        // Send byte 0x55 via UART
        Serial.write(0x55);
      }
    }
  }

  // Save the reading for the next loop
  lastButtonState = reading;
}