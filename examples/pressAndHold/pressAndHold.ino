// (c) Copyright 2022 Aaron Kimball
// This file is open source under the terms of the BSD 3-Clause license.
//
// Example usage for the debounce library: defining a button the user must hold down
// for a full second to trigger the effect.

#include<Arduino.h>
#include<debounce.h>

// Connect a button to some GPIO pin; digital pin 6 in this example.
static constexpr int PIN = 6;

static void buttonHandler(uint8_t btnId, uint8_t btnState) {
  if (btnState == BTN_PRESSED) {
    return; // Do nothing on press-down.
  }

  // btnState == BTN_OPEN.
  Serial.println("Pushed for long enough, then released button.");
}

static Button myButton(0, buttonHandler);

void setup() {
  Serial.begin(115200);
  pinMode(PIN, INPUT_PULLUP);

  // Must hold button down for 1s to trigger; a short press won't cut it.
  // This means it will take a second to trigger buttonHandler(..., BTN_PRESSED).
  // It will then take only the default 25ms to trigger buttonHandler(..., BTN_OPEN).
  myButton.setPushDebounceInterval(1000);

  // You can call setReleaseDebounceInterval() to configure the delay between when the
  // user lets go, and when your function is called, although too long a time delay on
  // release feels like a "sluggish" interface.
}

static void pollButtons() {
  myButton.update(digitalRead(PIN));
}

void loop() {
  pollButtons();
  delay(10);
}

