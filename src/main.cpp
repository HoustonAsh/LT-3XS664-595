#include <Arduino.h>
#include "Display.h"

Display display(3, 5, 7);

void setup() {
  display.setup();
}

void loop() {
  display
    .updateRows("Hello", "World", "!")
    .apply();

  delay(1000);

  display
    .updateRows("Worl", "Hello", "!")
    .apply();

  delay(1000);
}
