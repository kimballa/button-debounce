// (c) Copyright 2024 Aaron Kimball
// This file is open source under the terms of the BSD 3-Clause license.
//
// Example usage for the debounce library: taking a different action on release
// depending on whether the button was pressed for a short or a long time.
//
// Unlike the multiActionBtn example, only one of the two messages is sent for
// any given press: a single `Button` fires its handler on press and again on
// release, and on release the handler calls Button::priorStateDuration() to find
// out how long the button had been held down.

#include<Arduino.h>
#include<debounce.h>

// Connect a button to some GPIO pin; digital pin 6 in this example.
static constexpr int PIN = 6;

// A press held at least this long (in ms) counts as a "long" press.
static constexpr uint32_t LONG_PRESS_MILLIS = 1000;

static Button btn;

static void buttonHandler(uint8_t btnId, uint8_t btnState) {
  if (btnState == BTN_PRESSED) {
    // Nothing to do yet; we don't know whether this will be a short or long press.
    return;
  }

  // btnState == BTN_OPEN. The button has just been released; find out how long
  // it was in the BTN_PRESSED state before this.
  uint32_t pressMillis = btn.priorStateDuration();
  if (pressMillis >= LONG_PRESS_MILLIS) {
    Serial.print("Long press (");
  } else {
    Serial.print("Short press (");
  }
  Serial.print(pressMillis);
  Serial.println(" ms)");
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN, INPUT_PULLUP);

  btn.setHandler(buttonHandler);
}

void loop() {
  btn.update(digitalRead(PIN));
  delay(10);
}
