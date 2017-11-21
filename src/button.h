/**
 * Button.h
 * Scott Chase Waggener
 * 11/20/17
 * 
 * 
 * This library provides the functionality of a general purpose GPIO button to an
 * ESP8266 running Arduino framework. The button is controlled using interrupts
 * 
 */

#ifndef SOBUTTON_H
#define SOBUTTON_H


#include "Arduino.h"
#include <functional>
#include <FunctionalInterrupt.h>  // For std::bind ISR

typedef uint8_t gpio_num_t;
typedef std::function<void()> button_callback_t;


class Button {

  public:

    /**
     * @brief Constructs a button attached to the given pin
     * 
     * @param pin The GPIO to which the button will be attached
     */
    Button(const gpio_num_t pin);


    /**
     * @brief Detaches interrupts before destructing the button
     * 
     */
    ~Button();



    /**
     * @brief Gets the pin to which this button is attached
     * 
     * @return The GPIO
     */
    const gpio_num_t pin() const { return _pin; }


    /**
     * @brief Sets the callback to be run on a button press
     * 
     * @param func A pointer to the callback
     * 
     * @return this
     */
    Button &pressCallback(button_callback_t& func);


    /**
     * @brief Sets the callback to be run on a button hold
     * 
     * @param func A pointer to the callback
     * 
     * @return this
     */
    Button &holdCallback(button_callback_t& func);


    /**
     * @brief Sets the duration that distinguishes a button press from a button
     * hold.
     * 
     * @param time_ms The time in milliseconds
     * 
     * @return this
     */
    Button &holdDuration(byte button, unsigned long duration);
    const unsigned long holdDuration() { return _holdDuration_ms; }


    /**
     * @brief Sets the interval in ms to follow a button press during which
     * subsequent button presses will be ignored. This serves as a debounce
     * tool
     * 
     * @param time The time in milliseconds
     * 
     * @return this
     */
    Button &refractoryPeriod(unsigned long time_ms);
    const unsigned long refractoryPeriod() { return _refractory_ms; }
    

  private:

    const gpio_num_t _pin;
    const bool _RESTING_STATE;

    button_callback_t *_pressCallback = nullptr, *_holdCallback = nullptr;

    unsigned long _refractory_ms = 100; // Debouncing period
    unsigned long _holdDuration_ms = 5000;



    /**
     * @brief Member ISR function. This is called using std::bind with this as
     * a parameter. It distinguishes direction of button change and handles debounce
     * 
     */
    void _ISR();

    /**
     * @brief Detects the nondepressed GPIO state. Called in the initializer list. 
     * 
     */
    static const bool _findRestingState(gpio_num_t pin);

};

#endif
