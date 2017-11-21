#include "button.h"


Button::Button(const gpio_num_t p) :
_pin(p),
_RESTING_STATE(_findRestingState(p))
{
  pinMode(p,INPUT);
 
  attachInterrupt(p, std::bind(&Button::_ISR, this), CHANGE);  
}

Button::~Button() {
  detachInterrupt(_pin);
}


Button &Button::pressCallback(button_callback_t &func) {
  _pressCallback = &func;
  return *this;
}

Button &Button::holdCallback(button_callback_t &func) {
  _holdCallback = &func;
  return *this;
}

void Button::_ISR() {
  
  static volatile unsigned long last_event = 0;
  const volatile unsigned long ELAPSED_MS = abs(millis() - last_event);

  // Debounce
  if (ELAPSED_MS < _refractory_ms) {
    return;
  }

  // Act only on button release
  if(digitalRead(_pin) == _RESTING_STATE) {
    if( ELAPSED_MS >= _holdDuration_ms && _holdCallback) (*_holdCallback)();
    else if(_pressCallback) (*_pressCallback)();
  }

  last_event = millis();

}

const bool Button::_findRestingState(gpio_num_t pin) {
  pinMode(pin, INPUT);
  return digitalRead(pin);
}


