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
  Button();
  Button(uint8_t id, buttonHandler_t handlerFn);

  /**
   * Called to tell the button about the latest polled signal and register
   * as a press/release if appropriate.
   *
   * Returns true if the state has decisively changed, false otherwise.
   */
  bool update(uint8_t latestPoll);

  /** Returns 0 if button pressed, 1 if open. */
  uint8_t getState() const { return _curState; };

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
  unsigned int _pushDebounceInterval;
  unsigned int _releaseDebounceInterval;
  buttonHandler_t _handlerFn;
};

#ifdef __cplusplus
extern "C" {
#endif // C++
  void emptyBtnHandler(uint8_t btnId, uint8_t btnState);
#ifdef __cplusplus
};
#endif // C++

#endif /* _BUTTONS_H_ */
