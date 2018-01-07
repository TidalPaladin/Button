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
typedef uint8_t gpio_mode_t;
typedef std::function<void()> button_callback_t;


class Button {

  public:

    /**
     * @brief Constructs a button attached to the given pin
     * 
     * @param pin The GPIO to which the button will be attached
     * @param mode Pass INPUT or INPUT_PULLUP to flip button state
     * @param released_state The GPIO state when the button is released
     * 
     */
    Button(gpio_num_t pin, gpio_mode_t mode = INPUT, uint8_t released_state = HIGH);


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
    gpio_num_t pin() const { return _PIN; }

    /**
     * @brief Reads the value of the GPIO on which this button
     * is attached
     * 
     * @return HIGH or LOW
     */
    uint8_t read() const { return digitalRead(_PIN); }

    /**
     * @brief Detects whether the button is currently pressed
     * 
     * @return
     *   - true if the button is in a pressed down state
     *   - false otherwise
     */
    bool isPressed() const { return read() != _RESTING; }

    /**
     * @brief Sets the callback to be run on a button press
     * 
     * @param func A std::function object / lambda function
     * 
     * @return this
     */
    Button &pressCallback(button_callback_t func);
    bool pressCallback() { return _pressCallback == nullptr; }


    /**
     * @brief Sets the callback to be run on a button hold
     * 
     * @param func A std::function object / lambda function
     * 
     * @return this
     */
    Button &holdCallback(button_callback_t func);
    bool holdCallback() { return _holdCallback == nullptr; }

    /**
     * @brief Sets the callback to run when the button changes state.
     * 
     * @note The execution of this callback is subject to the refractory
     * period. Very short button presses will call the change callback on
     * press down but not on release.
     * 
     * @param func A std::function object / lambda function
     * 
     */
    Button &changeCallback(button_callback_t func);
    bool changeCallback() { return _changeCallback == nullptr; }


    /**
     * @brief Sets the duration that distinguishes a button press from a button
     * hold.
     * 
     * @param time_ms The time in milliseconds
     * 
     * @return this
     */
    Button &holdDuration(unsigned long time_ms);
    unsigned long holdDuration() { return _holdDuration_ms; }


    /**
     * @brief Sets the interval in ms to follow a button press during which
     * subsequent button presses will be ignored. This serves as a debounce
     * tool
     * 
     * @param time_ms The time in milliseconds
     * 
     * @return this
     */
    Button &refractoryPeriod(unsigned long time_ms);
    unsigned long refractoryPeriod() { return _refractory_ms; }

    /**
     * @brief Operator bool. 
     * 
     * Returns true if a valid callback exists. False otherwise.
     */
    operator bool() { return _pressCallback || _holdCallback || _changeCallback; }
    

  private:

    const gpio_num_t _PIN;
    const uint8_t _RESTING;

    button_callback_t _pressCallback, _holdCallback, _changeCallback;

    unsigned long _refractory_ms = 100; // Debouncing period
    unsigned long _holdDuration_ms = 5000;


    /**
     * @brief Member ISR function. This is called using a std::function<void()> object
     * It distinguishes direction of button change and handles debounce
     * 
     */
    void _ISR();


};

#endif
