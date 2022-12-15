// (c) Copyright 2022 Aaron Kimball
// This file is open source under the terms of the BSD 3-Clause license.
//
// Example basic usage for the debounce library.

#include<Arduino.h>
#include<debounce.h>

// Connect a button to some GPIO pin; digital pin 6 in this example.
static constexpr int PIN = 6;

static void buttonHandler(uint8_t btnId, uint8_t btnState) {
  if (btnState == BTN_PRESSED) {
    Serial.println("Pushed button");
  } else {
    // btnState == BTN_OPEN.
    Serial.println("Released button");
  }
}

// Define your button with a unique id (0) and handler function.
// (The ids are so one handler function can tell different buttons apart if necessary.)
static Button myButton(0, buttonHandler);

void setup() {
  Serial.begin(115200);
  pinMode(PIN, INPUT_PULLUP);
}

static void pollButtons() {
  // update() will call buttonHandler() if PIN transitions to a new state and stays there
  // for multiple reads over 25+ ms.
  myButton.update(digitalRead(PIN));
}

void loop() {
  pollButtons(); // Poll your buttons every loop.
  delay(10); // Loop iterations should run fairly quickly, 10's of ms, not 100's.
}

