const unsigned int
  SENSOR_POWER_PIN = 7,
  RELAY_SIGNAL_OUTPUT_PIN = 4,
  RELAY_POWER_OUTPUT_PIN = 8,
  SIGNAL_PIN = A5,
  BUTTON_PIN = 2,
  RED_LED_PIN = 13,
  GREEN_LED_PIN = 12;

void setup() 
{
  Serial.begin(9600);

  pinMode(SENSOR_POWER_PIN, OUTPUT);  // configure D7 pin as an OUTPUT
  pinMode(RELAY_SIGNAL_OUTPUT_PIN, OUTPUT);
  pinMode(RELAY_POWER_OUTPUT_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  pinMode(BUTTON_PIN, INPUT);

  digitalWrite(SENSOR_POWER_PIN, LOW);  // turn the sensor OFF
  digitalWrite(RELAY_SIGNAL_OUTPUT_PIN, LOW);
  digitalWrite(RELAY_POWER_OUTPUT_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
}

int sensorValue = 0;  // variable to store the sensor value
int waterCheckValue = 100; // this value is needed to check if there's water on the sensor
bool isButtonPressed{}, isRelayOn{};

void loop() 
{
  isButtonPressed = digitalRead(BUTTON_PIN);

  Serial.print("Button: ");
  Serial.println(isButtonPressed);

  if (isButtonPressed)
  {
    Serial.print("Button pressed.\n");
    digitalWrite(SENSOR_POWER_PIN, HIGH);  // turn the sensor ON

    delay(10);  // wait 10 milliseconds

    do
    {
      sensorValue = analogRead(SIGNAL_PIN);  // read the analog value from sensor
      Serial.print("Sensor value: ");
      Serial.println(sensorValue);
      
      // if there's water
      if (sensorValue >= waterCheckValue)
      {
        digitalWrite(RELAY_SIGNAL_OUTPUT_PIN, LOW);
        digitalWrite(RELAY_POWER_OUTPUT_PIN, LOW);
        Serial.println("Relay turned OFF");
        digitalWrite(GREEN_LED_PIN, LOW);

        if (isRelayOn)
        {
          digitalWrite(RED_LED_PIN, HIGH);
          delay(500);

          for (int i = 0; i < 3; i++) 
          {
            digitalWrite(RED_LED_PIN, HIGH);
            digitalWrite(GREEN_LED_PIN, LOW);
            delay(500);
            digitalWrite(RED_LED_PIN, LOW);
            digitalWrite(GREEN_LED_PIN, HIGH);
            delay(400);
          }
          digitalWrite(GREEN_LED_PIN, LOW);
        }
        else
        {
          for (int i = 0; i < 5; i++) 
          {
            digitalWrite(RED_LED_PIN, HIGH);
            delay(300);
            digitalWrite(RED_LED_PIN, LOW);
            delay(250);
          }
        }
        
        isRelayOn = false;

        break;
      }
      // if button was pressed when the relay is on
      else if (digitalRead(BUTTON_PIN) && isRelayOn)
      {
        // turning relay off:
        digitalWrite(RELAY_SIGNAL_OUTPUT_PIN, LOW);
        digitalWrite(RELAY_POWER_OUTPUT_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, LOW);
        digitalWrite(SENSOR_POWER_PIN, LOW);

        for (int i = 0; i < 5; i++)
        {
          digitalWrite(RED_LED_PIN, HIGH);
          digitalWrite(GREEN_LED_PIN, LOW);
          delay(200);
          digitalWrite(RED_LED_PIN, LOW);
          digitalWrite(GREEN_LED_PIN, HIGH);
          delay(200);
        }
        digitalWrite(GREEN_LED_PIN, LOW);

        isRelayOn = false;

        break;
      }
      // if relay isn't already on:
      else if (!isRelayOn)
      {
        digitalWrite(RELAY_POWER_OUTPUT_PIN, HIGH);
        delay(10);
        digitalWrite(RELAY_SIGNAL_OUTPUT_PIN, HIGH);
        digitalWrite(GREEN_LED_PIN, HIGH);

        isRelayOn = true;
        Serial.println("Relay turned ON");
      }

      delay(1500);
    } while (sensorValue < waterCheckValue);
    
  }
  else 
  {
    digitalWrite(SENSOR_POWER_PIN, LOW);  // turn the sensor OFF
    digitalWrite(RELAY_SIGNAL_OUTPUT_PIN, LOW);
    digitalWrite(RELAY_POWER_OUTPUT_PIN, LOW);

    delay(1000);
  }
}
