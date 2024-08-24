// (c) Copyright 2024 Aaron Kimball
// This file is open source under the terms of the BSD 3-Clause license.
//
// Example usage for the debounce library: defining multiple short- and 
// long-press actions on the same button.
//
// Multiple `Button` instances can be signaled by the same GPIO pin / physical
// switch. By configuring different debounce intervals on different `Button`
// instance with the same switch, separate "short press" and long press" actions
// can be taken.

#include<Arduino.h>
#include<debounce.h>

// Connect a button to some GPIO pin; digital pin 6 in this example.
static constexpr int PIN = 6;

static void shortPressHandler(uint8_t btnId, uint8_t btnState) {
  if (btnState == BTN_PRESSED) {
    // Send a message as soon as the button is pressed.
    Serial.println("Button pressed a short time");
  } else {
    // btnState == BTN_OPEN.
    // Do nothing on button release.
    return; 
  }
}

static void longPressHandler(uint8_t btnId, uint8_t btnState) {
  if (btnState == BTN_PRESSED) {
    // Send a message after it has been held down a long time.
    Serial.println("Button pressed and held a long time");
  } else {
    // btnState == BTN_OPEN.
    // Do nothing on button release.
    return;
  }
}

static void longPressAndReleaseHandler(uint8_t btnId, uint8_t btnState) {
  if (btnState == BTN_PRESSED) {
    Serial.println("Button held down... rocket launch armed.");
  } else {
    // btnState == BTN_OPEN.
    // Shown when the button is released, but only if held down 3s first.
    Serial.println("Launching the rocket!");
  }
}


static Button shortBtn(0, shortPressHandler);
static Button longBtn(1, longPressHandler);
static Button longPressReleaseBtn(2, longPressAndReleaseHandler);

void setup() {
  Serial.begin(115200);
  pinMode(PIN, INPUT_PULLUP);

  // shortBtn stays with the default 25ms (`BTN_DEBOUNCE_MILLIS`) debounce time interval.

  // Must hold button down for 1s to trigger `longPressHandler()`; a short press won't cut it.
  // This means it will take a second to trigger buttonHandler(..., BTN_PRESSED).
  // It will then take only the default 25ms to trigger buttonHandler(..., BTN_OPEN).
  longBtn.setPushDebounceInterval(1000);  // 1000 ms = 1 second

  // Must hold button down for 3s to trigger `longPressHandler()`. That will send
  // the "rocket launch armed" message. After the user releases the button, the
  // "Launching the rocket" message will be sent, but only if the button was held
  // long enough to receive the "armed" message first.
  longPressReleaseBtn.setPushDebounceInterval(3000);  // 3 seconds


  // You can call setReleaseDebounceInterval() to configure the delay between when the
  // user lets go and when your function is called, although too long a time delay on
  // release feels like a "sluggish" interface.
}

static void pollButtons() {
  uint8_t pinState = digitalRead(PIN); // We only read one GPIO pin...

  // ... But we use it to trigger three button handler functions.
  shortBtn.update(pinState);
  longBtn.update(pinState);
  longPressReleaseBtn.update(pinState);
}

void loop() {
  pollButtons();
  delay(10);
}

