#include "Button.h"


Button::Button(const gpio_num_t p, const gpio_mode_t mode, const uint8_t released_state) :
_PIN(p),
_RESTING(released_state)
{
  pinMode(p, mode);

  // Attach using lambda function instead of std::bind
  // std::bind(&Button::_ISR, this)
  attachInterrupt(
    p, 
    [this]() { this->_ISR(); },
    CHANGE
  );  
}
 

Button::~Button() {
  detachInterrupt(_PIN);
}


Button &Button::onPress(const button_callback_t &func) {
  _pressCallback = func;
  return *this;
} 

Button &Button::onHold(const button_callback_t &func) {
  _holdCallback = func;
  return *this;
}

Button &Button::onChange(const button_callback_t &func) {
  _changeCallback = func;
  return *this;
}

Button &Button::holdDuration(const unsigned long time_ms) {
  _holdDuration_ms = time_ms;
  return *this;
}

Button &Button::refractoryPeriod(const unsigned long time_ms) {
  _refractory_ms = time_ms;
  return *this;
}

void Button::_ISR() {
  
  static unsigned long last_event = 0;
  const unsigned long ELAPSED_MS = millis() - last_event;

  // Debounce
  if (ELAPSED_MS < _refractory_ms) {
    return;
  }

  // Capture state of pin and update time
  const bool RELEASED = ( digitalRead(_PIN) == _RESTING );
  last_event = millis();

  // Run the change callback no matter what
  if(_changeCallback) {
    (_changeCallback)();
  }

  // Run press / hold on button release
  if( RELEASED ) {
    if( ELAPSED_MS < _holdDuration_ms && _pressCallback) (_pressCallback)();
    else if(_holdCallback) (_holdCallback)();
  }

}

