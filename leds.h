int BLUE = 2;
int RED = 3;
int GREEN = 5;

int allPins[] = { 2, 3, 5 };

void initLEDS() {
  for (int pin : allPins) {
    pinMode(pin, INPUT_PULLUP);
    digitalWrite(pin, LOW);
    Serial.print("Set pin ");
    Serial.print(pin);
    Serial.println(" to INPUT_PULLUP");
  }
}