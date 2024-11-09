#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display width and height
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Create an OLED display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int ledPin = 6;
const int buzzerPin = 9;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Initialize the OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  clearScreen();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
}

void loop() {
  // Check for incoming serial data
  if (Serial.available()) {
    String incomingString = Serial.readString(); 
    // Existing code to handle "FB" in the incoming string
    if (incomingString.indexOf("FB") > 0) {
      Serial.println("Works");
      
      digitalWrite(ledPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      delay(300);
      digitalWrite(ledPin, LOW);
      digitalWrite(buzzerPin, LOW);
      delay(300);
      digitalWrite(ledPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      delay(300);
      digitalWrite(ledPin, LOW);
      digitalWrite(buzzerPin, LOW);
      delay(300);
      displayHitMessage();
    }
  }
}




void displayHitMessage() {
  display.fillRect(0, 0, 128, 30, SSD1306_BLACK); // Clear the top portion of the screen
  display.setCursor(10, 0);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(3);
  display.print("HIT");
  display.display();
  delay(2000); // Display "HIT" for 4 seconds
  display.fillRect(10, 0, 108, 30, SSD1306_BLACK); // Clear the "HIT" message
  display.setTextColor(SSD1306_WHITE);
  display.display();
}
void clearScreen() {
  // Clear the screen to remove any fuzziness
  display.clearDisplay();
  display.display();
  delay(100);
  display.clearDisplay();
  display.display();
}
