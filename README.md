
debounce
========

Button signal processing and debouncing.

Buttons are mapped to handler functions which are called when the button has reliably
transitioned to the down (pressed) or up (released) positions. Glitches are filtered out
by this debouncing library.

Button inputs may be read directly from GPIO, over PCF8574 I2C bus (via libi2cparallel:
https://github.com/kimballa/i2cparallel), or any other method. You should monitor the
hardware signals in a polling loop. Each time you receive a new signal input sample, use
`Button::update(signalValue)` to transmit the new sample.  The `Button` class itself does
not directly monitor a hardware signal input. The `update()` method will fire your handler
method (of type `buttonHandler_t`) when a clear state change has been monitored:

```
void pollButtons() {
  myButton.update(digitalRead(MY_BTN_PIN));
  pcfButton.update(myI2CParallel.read() & 0x01); // update from lsb of I2C 8-bit bus.
}

void loop() {
  pollButtons();
  // Do other stuff
  delay(25); // wait 25 ms.
}
```

Each button is assigned a unique id (which can e.g. be the same as its index within a
`vector<Button>` array), which is passed to the handler function along with the button
state (`BTN_OPEN` or `BTN_PRESSED`):

```
// myBtnHandler is a fn ptr of type buttonHandler_t with this signature:
void myBtnHandler(uint8_t btnId, uint8_t btnState) {
  if (btnState == BTN_PRESSED) {
    // Handle keypress of the particular button.
  }
}
```

Compiling
---------

Find the `debounce` library in the Arduino IDE and add it to your project; make sure to
`#include <debounce.h>`.

Alternatively, I build this with my [Arduino
makefile](https://github.com/kimballa/arduino-makefile):

* Clone the makefile project such that `arduino-makefile/` is a sibling of this project directory.
* Create `~/arduino_mk.conf` from the template in that directory and customize it to your board
  and local environment. See other one-time setup instructions in that project's README.md and/or
  the comment header of `arduino.mk`.
* Build this library with `make install`
* For other arduino-makefile-based projects, add this to your project's dependencies with
  `libs := debounce`

License
-------

This project is licensed under the BSD 3-Clause license. See LICENSE.txt for complete details.

