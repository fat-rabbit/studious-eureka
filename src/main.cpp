#include <Arduino.h>
#include <algorithm>

enum LedState
{
  ON,
  OFF
};

unsigned long lastRead_timestamp = 0;
unsigned long lastPressed_timestamp = 0;

constexpr uint8_t ADC_PIN = 9;
constexpr uint8_t BUTTON_PIN = 8;
constexpr uint8_t LED_PIN = 17;

constexpr uint8_t intervalMs = 100;
constexpr uint8_t debounceMs = 50;

uint8_t buttonState = LOW;
LedState ledState = ON;

void setup()
{
  Serial.begin(9600);
  pinMode(ADC_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
}

uint32_t calculateVoltage(int16_t raw)
{
  float value = 3.3 * (raw / 4095.0);
  return (uint32_t)(value * 1000);
}

uint8_t calculateError(uint32_t real, uint32_t expected)
{
  if (expected == 0)
  {
    return 100;
  }
  
  float delta = (float)(real - expected) / expected;
  if (delta < 0)
  {
    delta = -delta;
  }
  return (uint8_t)(delta * 100);
}

void loop()
{
  ulong current_timestamp = millis();
  uint8_t current_button_state = digitalRead(BUTTON_PIN);

  if (current_timestamp - lastRead_timestamp >= intervalMs)
  {
    uint16_t raw = analogRead(ADC_PIN);
    uint32_t measuredVoltage = analogReadMilliVolts(ADC_PIN);
    uint32_t calculatedVoltage = calculateVoltage(raw);
    uint8_t error = calculateError(measuredVoltage, calculatedVoltage);
    Serial.printf("Raw=%d; V_calc=%d; V_measured=%d; Error=%d%;\n", raw, calculatedVoltage, measuredVoltage, error);
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
