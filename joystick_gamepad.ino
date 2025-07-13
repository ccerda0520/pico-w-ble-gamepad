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
  Serial.begin(115200);
  Serial.println("Start Joystick demo.");
  JoystickBLE.begin("Pico Joystick");

  // setting to pullup resistors so that a button press is registered
  for (int i=0; i<11; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  for (int i=0; i<4; i++) {
    pinMode(joyPins[i], INPUT_PULLUP);
  }
}

void loop() {
  // batching the instructions to prevent packet loss / overload
  JoystickBLE.useManualSend(true);

  // // Check if the button is pressed (LOW means pressed because of pull-up resistor)
  for (int i=0; i<11; i++) {
    bool pressed = digitalRead(buttonPins[i]) == LOW;  // active low
    JoystickBLE.button(buttonIds[i], pressed);

    if (pressed) {
      Serial.print("Button is pressed: ");
      Serial.println(buttonIds[i]);
    }
  }

  // digital joystick mapping
  for (int i=0; i<4; i++) {
    bool joyPressed = digitalRead(joyPins[i]) == LOW;
    JoystickBLE.button(joyButtonIds[i], joyPressed);

    if (joyPressed) {
      Serial.println((String)"joy Button is pressed: " + joyButtonIds[i]);
    }
  }
  
  JoystickBLE.send_now();
  delay(50);
}
