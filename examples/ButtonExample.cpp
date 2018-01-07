/**
 * Button example
 * Scott Chase Waggener
 * 12/16/17
 * 
 * This example demonstrates the basic use of "Button.h"
 * Written for ESP8266 but likely compatible with Arduino
 *
 */

#include <Arduino.h>
#include "Button.h"

// Create a button object on GPIO0
Button b(0, INPUT_PULLUP);


void setup() {
  Serial.begin(115200); Serial.println("\r\n\r\n");

  pinMode(LED_BUILTIN, OUTPUT);

  // Holding longer than 3000 ms will trigger a button hold callback
  b.holdDuration(3000); // Default 5000

  // Ignore button events less than 200 ms apart (debouncing)
  b.refractoryPeriod(50);  // Default 100

  // Create a callback to run on button press
  b.pressCallback([&]() {
    /**
     * NOTE
     * Callbacks are run inside an ISR, so ISR rules apply:
     *  No excessively long operations
     *  Use volatiles where needed
     * 
     * Serial should probably be avoided too
     */
    Serial.println("Button pressed!");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  });

  // Create a callback to run on button hold
  b.holdCallback([&]() {
    Serial.println("Button held!");
  });

  // Create a callback to run when the button changes state.
  // This is run before the press / hold callback.
  // Depending on how long callbacks take, state functions of Button
  // may return out of date results. 
  b.changeCallback([]() {
    if( b.isPressed() )
      Serial.println("The button went down");
    else
      Serial.println("The button went up");
    Serial.println( b.read() );
  });
  
}

void loop() {
  
}
