// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
String lora_RX_address = "2";
const int sensorPin = 9;
const int impactThreshold = 100; // Adjust this value based on your needs
bool impactDetected = false;
unsigned long previousTime = 0;
float previousMagnitude = 0;

Adafruit_MPU6050 mpu;
#include <Encoder.h>

// Rotary Encoder Pins
#define CLK 2
#define DT 3

// Create an Encoder object
Encoder myEnc(CLK, DT);

long oldPosition  = -999;
unsigned long previousMillis = 0; // Store the last time the position was printed
const long interval = 5000; // Interval at which to print the position (5 seconds)

void setup() {
  // Initialize serial communication for the encoder
  Serial.begin(9600);

pinMode(sensorPin, INPUT);
  Serial.begin(9600);

  
}
void loop() {


    // Read the current position of the encoder
  long newPosition = -myEnc.read() / 199.895; // Adjust the divisor based on your encoder's resolution

  // Get the current time
  unsigned long currentMillis = millis();

  // If the position has changed, print the new position
  if (newPosition != oldPosition || (currentMillis - previousMillis >= interval)) {
    oldPosition = newPosition;
    previousMillis = currentMillis;

    // Map the position to a clock face (0 to 11)
    int clockPosition = newPosition % 12;
    if (clockPosition < 0) {
      clockPosition += 12;
    }

    // Format the clock position to always display two digits
    char formattedClockPosition[4];
    sprintf(formattedClockPosition, "C%02d", clockPosition);

    Serial.print("Encoder Position: ");
    Serial.print(newPosition);
    Serial.print(" | Clock Position: ");
    Serial.println(formattedClockPosition);

    // Send the clock position via LoRa
    String loraCommand = "AT+SEND=" + (lora_RX_address) + (",") + ("3") + (",") +  (String(formattedClockPosition));
    Serial.println(loraCommand);
    delay(100);
    Serial.println(loraCommand);
    delay(100);
    Serial.println(loraCommand);
    delay(100);
    Serial.println(loraCommand);
    delay(100);
  }


   // Adjust the delay as needed
}
