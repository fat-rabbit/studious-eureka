#include <Arduino.h>

enum LedState
{
  ON,
  OFF
};

unsigned long lastRead_timestamp = 0;
unsigned long lastPressed_timestamp = 0;

constexpr uint8_t ACD_PIN = 13;
constexpr uint8_t BUTTON_PIN = 8;
constexpr uint8_t LED_PIN = 17;

constexpr uint8_t intervalMs = 50;
constexpr uint8_t debounceMs = 50;

uint8_t buttonState = LOW;
LedState ledState = ON;

void setup()
{
  Serial.begin(9600);
  pinMode(ACD_PIN, ANALOG);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
}

void loop()
{
  ulong current_timestamp = millis();
  uint8_t current_button_state = digitalRead(BUTTON_PIN);

  if (current_timestamp - lastRead_timestamp >= intervalMs)
  {
    uint16_t analogSignal = analogRead(ACD_PIN);
    Serial.println(analogSignal);
    lastRead_timestamp = current_timestamp;
  }

  if (current_timestamp - lastPressed_timestamp >= debounceMs)
  {
    if (current_button_state == LOW && buttonState == HIGH)
    {
      Serial.println("Turn Off the Led");
      ledState = OFF;
      digitalWrite(LED_PIN, LOW);
    }
    else if (current_button_state == HIGH && buttonState == LOW)
    {
      Serial.println("Turn On the Led");
      ledState = ON;
      digitalWrite(LED_PIN, HIGH);
    }
    buttonState = current_button_state;
    lastPressed_timestamp = current_timestamp;
  }
}