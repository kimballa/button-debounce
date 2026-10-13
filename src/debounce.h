// (c) Copyright 2022 Aaron Kimball

#ifndef _BUTTON_DEBOUNCE_H_
#define _BUTTON_DEBOUNCE_H_

#include<Arduino.h>

/* 25 ms delay for a button press to be registered as "valid." */
constexpr unsigned int BTN_DEBOUNCE_MILLIS = 25;

// Button::getState() returns 0 if a button is registered as pressed, 1 if open.
constexpr uint8_t BTN_PRESSED = 0;
constexpr uint8_t BTN_OPEN = 1;

// A function called whenever a button has definitively changed state.
typedef void (*buttonHandler_t)(uint8_t id, uint8_t btnState);

class Button {
public:
  Button() : Button(0, NULL) {};
  Button(uint8_t id, buttonHandler_t handlerFn):
      _id(id), _curState(BTN_OPEN), _priorPoll(BTN_OPEN), _readStartTime(0),
      _stateStartTime(0), _priorStateDuration(0),
      _pushDebounceInterval(BTN_DEBOUNCE_MILLIS),
      _releaseDebounceInterval(BTN_DEBOUNCE_MILLIS),
      _handlerFn(handlerFn) {

    if (NULL == _handlerFn) {
      _handlerFn = emptyBtnHandler;
    }
  };

  /**
   * Called to tell the button about the latest polled signal and register
   * as a press/release if appropriate.
   *
   * Returns true if the state has decisively changed, false otherwise.
   */
  bool update(uint8_t latestPoll) {
    latestPoll = (latestPoll != 0); // Collapse input into a 1/0 universe.

    if (latestPoll != _priorPoll) {
      // Input has changed since we last polled. Reset debounce timer.
      _readStartTime = millis();
    }

    // Save reading for next interrogation of update().
    _priorPoll = latestPoll;

    // Decide which debounce interval to use, depending on whether we're monitoring
    // for a next state change of "push" (0 to 1) or "release" (1 to 0).
    unsigned int debounceInterval = _pushDebounceInterval;
    if (_curState == BTN_PRESSED) {
      debounceInterval = _releaseDebounceInterval;
    }

    if ((millis() - _readStartTime) > debounceInterval) {
      // The reading has remained consistent for the debounce interval.
      // It's a legitimate state.

      if (latestPoll != _curState) {
        // Lock this in as the new state, remembering how long the old one lasted.
        uint32_t now = millis();
        _priorStateDuration = now - _stateStartTime;
        _stateStartTime = now;
        _curState = latestPoll;
        (*_handlerFn)(_id, _curState); // Invoke callback handler.
        return true;
      }
    }

    return false; // No state change.
  };

  /** Returns 0 if button pressed, 1 if open. */
  uint8_t getState() const { return _curState; };

  /**
   * Returns how long (in ms) the button was in its *prior* state, i.e. the time
   * between the two most recent registered state changes. Intended to be called
   * from the handler: on a BTN_OPEN event it gives the duration of the press.
   *
   * You can use this to take a different action on release after a short press
   * vs a long press.
   *
   * Returns time since startup if no prior state change has been registered yet.
   * The duration includes the debounce interval.
   */
  uint32_t priorStateDuration() const { return _priorStateDuration; };

  void setHandler(buttonHandler_t handlerFn) { _handlerFn = handlerFn; };
  const buttonHandler_t getHandler() const { return _handlerFn; };

  unsigned int getPushDebounceInterval() const { return _pushDebounceInterval; };
  void setPushDebounceInterval(unsigned int debounce) { _pushDebounceInterval = debounce; };
  void setReleaseDebounceInterval(unsigned int debounce) { _releaseDebounceInterval = debounce; };

private:
  uint8_t _id;
  uint8_t _curState;
  uint8_t _priorPoll;
  uint32_t _readStartTime;
  uint32_t _stateStartTime; // When _curState was registered.
  uint32_t _priorStateDuration; // How long the state before _curState lasted.
  unsigned int _pushDebounceInterval;
  unsigned int _releaseDebounceInterval;
  buttonHandler_t _handlerFn;
};

/** A button handler that does nothing; for states when a given button is unmapped. */
inline void emptyBtnHandler(uint8_t btnId, uint8_t btnState) {
}

#endif /* _BUTTONS_H_ */
