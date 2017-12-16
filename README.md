# Arduino Button

This button library simplifies the implementation of GPIO based callbacks. The button object is assigned to a GPIO.
On assignment the object automatically detects and stores the resting state of the pin. Users can assign callbacks
to be run when the button is pressed or held. The duration that distinguishes a press from a hold can be set by the
user. Debouncing is handled by the library, with the debounce interval being user assignable.

Callbacks are run within an ISR, thus ISR best practices apply. Callbacks should be kept short, and `volatile` should
be used when necessary.


## Methods

** Constructor **
`Button b(const gpio_num_t); // Construct a button on a given GPIO`

** Setting callbacks **
Callbacks are passed as `std::function<void()>` objects and are copied into the `Button` object.

```
b.pressCallback([]() {
    // Do something on button press
});

b.holdCallback([]() {
    // Do something on hold
});
```

** Setting timings **
```
// Holding longer than 3000 ms will trigger a button hold callback
b.holdDuration(3000); // Default 5000

// Ignore button events less than 200 ms apart (debouncing)
b.refractoryPeriod(200);  // Default 100
```
