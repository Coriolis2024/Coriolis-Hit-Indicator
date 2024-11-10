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
// Mosfet pin as output
const int mosfetPin = 4; // The MOSFET is connected to digital pin 4

// Create an Encoder object
Encoder myEnc(CLK, DT);

long oldPosition  = -999;
unsigned long previousMillis = 0; // Store the last time the position was printed
const long interval = 5000; // Interval at which to print the position (5 seconds)

void setup() {
  // Initialize serial communication for the encoder
  pinMode(mosfetPin, OUTPUT);
  Serial.begin(9600);

pinMode(sensorPin, INPUT);
  Serial.begin(9600);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  
}
void loop() {
    /* Get new sensor events with the readings */
  
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");
long accelerationMagnitude = sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z);
unsigned long currentTime = millis();
float deltaTime = (currentTime - previousTime) / 1000.0;
if (deltaTime > 0) {
    float deltaA = (accelerationMagnitude - previousMagnitude) / deltaTime;
    if (abs(deltaA) > impactThreshold) {
       Serial.println("AT+SEND="+ lora_RX_address +",2,FB");
    delay(100);
    Serial.println("AT+SEND="+ lora_RX_address +",2,FB");
    delay(100);
    Serial.println("AT+SEND="+ lora_RX_address +",2,FB");
    delay(50);
          Serial.println("AT+SEND="+ lora_RX_address +",2,FB");
    delay(100);
    Serial.println("AT+SEND="+ lora_RX_address +",2,FB");
    delay(100);
    Serial.println("AT+SEND="+ lora_RX_address +",2,FB");
    delay(50);
    }
accelerationMagnitude = previousMagnitude;
previousTime = currentTime;
  delay(100);

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

 if (Serial.available() > 0) {
    // Read the incoming data as a string
    String receivedData = Serial.readString();

    // Check if the received data contains the characters "TR"
    if (receivedData.indexOf("TR") >= 0) {
      // Turn the MOSFET (and solenoid) on
      digitalWrite(mosfetPin, HIGH);
      delay(333); // Wait for 1/3 of a second (333 milliseconds)
      // Turn the MOSFET (and solenoid) off
      digitalWrite(mosfetPin, LOW);

      // Send the response to indicate the machine is in "record" mode
      Serial.println("AT+SEND=1,2,RE");
    }

    // Check if the received data contains the characters "ST"
    else if (receivedData.indexOf("ST") >= 0) {
      // Turn the MOSFET (and solenoid) on
      digitalWrite(mosfetPin, HIGH);
      delay(333); // Wait for 1/3 of a second (333 milliseconds)
      // Turn the MOSFET (and solenoid) off
      digitalWrite(mosfetPin, LOW);

      // Send the response to indicate the machine is in "stop" mode
      Serial.println("AT+SEND=1,2,ST");
    }
  }
}
