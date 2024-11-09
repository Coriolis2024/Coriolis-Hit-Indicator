#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int buttonPin = 2; // The button is connected to digital pin 2

bool isRecording = false;
unsigned long startTime = 0;
unsigned long elapsedTime = 0;
int minutes = 0;
int seconds = 0;

void setup() {
  // Initialize the button pin as an input
  pinMode(buttonPin, INPUT_PULLUP);

  // Begin serial communication at a baud rate of 9600
  Serial.begin(9600);

  // Initialize the OLED display with address 0x3C for 128x64
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.display();
}

void loop() {
  // Check if the button is pressed
  if (digitalRead(buttonPin) == LOW) {
    delay(50); // Debounce delay
    if (digitalRead(buttonPin) == LOW) { // Ensure it's still pressed
      if (isRecording) {
        Serial.println("AT+SEND=2,2,ST");
      } else {
        Serial.println("AT+SEND=2,2,TR");
      }
      while (digitalRead(buttonPin) == LOW); // Wait for the button to be released
    }
  }

  // Check for incoming serial data
  if (Serial.available() > 0) {
    String receivedData = Serial.readString();
    if (receivedData.indexOf("RE") >= 0) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.println("Recording");
      display.display();
      isRecording = true;
      startTime = millis();
    }
    if (receivedData.indexOf("ST") >= 0) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.println("Stopped");
      display.display();
      isRecording = false;
      startTime = millis();
    }
  }

  // Update the elapsed time for both "Recording" and "Stopped" modes
  elapsedTime = (millis() - startTime) / 1000;
  minutes = elapsedTime / 60;
  seconds = elapsedTime % 60;

  display.fillRect(SCREEN_WIDTH - 50, SCREEN_HEIGHT - 10, 50, 10, SSD1306_BLACK); // Clear previous time
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(SCREEN_WIDTH - 50, SCREEN_HEIGHT - 10);
  display.print(minutes);
  display.print(":");
  if (seconds < 10) {
    display.print("0"); // Display leading zero for single-digit seconds
  }
  display.print(seconds);
  display.display();
}
