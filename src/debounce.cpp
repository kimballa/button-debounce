// (c) Copyright 2022 Aaron Kimball

#include "debounce.h"

/** A button handler that does nothing; for states when a given button is unmapped. */
void emptyBtnHandler(uint8_t btnId, uint8_t btnState) {
}

Button::Button() : Button(0, NULL) {
}

Button::Button(uint8_t id, buttonHandler_t handlerFn):
    _id(id), _curState(BTN_OPEN), _priorPoll(BTN_OPEN), _readStartTime(0),
    _pushDebounceInterval(BTN_DEBOUNCE_MILLIS),
    _releaseDebounceInterval(BTN_DEBOUNCE_MILLIS),
    _handlerFn(handlerFn) {

  if (NULL == _handlerFn) {
    _handlerFn = emptyBtnHandler;
  }
}

bool Button::update(uint8_t latestPoll) {
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
      _curState = latestPoll; // Lock this in as the new state.
      (*_handlerFn)(_id, _curState); // Invoke callback handler.
      return true;
    }
  }

  return false; // No state change.
}

