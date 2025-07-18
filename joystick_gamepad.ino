#include <JoystickBLE.h>

// Define the GPIO pin where the button is connected
const int buttonPins[11] = {12,5,6,7,8,9,10,11,16,17,18};
const int buttonIds[11] = {5,6,7,8,9,10,11,12,13,14,15};
const int joyPins[4] = {0,1,2,3};  // UP, RIGHT, DOWN, LEFT
const int joyButtonIds[4] = {2,1,4,3};

// Variable to store the button state
int joystickXState = 0;
int joystickYState = 0;

void setup() {
  JoystickBLE.begin("Pico Joystick");

  // batching the instructions to prevent packet loss / overload
  JoystickBLE.useManualSend(true); 


  // setting to pullup resistors so that a button press is registered
  for (int i=0; i<11; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  for (int i=0; i<4; i++) {
    pinMode(joyPins[i], INPUT_PULLUP);
  }
}

unsigned long lastSend = 0;
unsigned long debounceDelay = 30; 

bool buttonLastStates[11] = {false};
bool joyLastStates[4] = {false};

void loop() {
  // only send ble report if something actually changed
  bool didChange = false;

  if (millis() - lastSend > debounceDelay) {
    // // Check if the button is pressed (LOW means pressed because of pull-up resistor)
    for (int i=0; i<11; i++) {
      bool pressed = digitalRead(buttonPins[i]) == LOW;  // active low

      if (pressed != buttonLastStates[i]) {
        JoystickBLE.button(buttonIds[i], pressed);
        buttonLastStates[i] = pressed;
        didChange = true;
      }
      
    }

    // digital joystick mapping
    for (int i=0; i<4; i++) {
      bool joyPressed = digitalRead(joyPins[i]) == LOW;
      
      if (joyPressed != joyLastStates[i]) {
        JoystickBLE.button(joyButtonIds[i], joyPressed);
        joyLastStates[i] = joyPressed;
        didChange = true;
      }
    }
    
    if (didChange) {
      JoystickBLE.send_now();
      lastSend = millis();
    }
  }
}
