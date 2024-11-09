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
    String incomingString = Serial.readString(); // Read until newline character
    
    if (incomingString.startsWith("+RCV=1,3,C")) { // Check if the line starts with "+RCV=1,3,C"
      Serial.print(incomingString);
      
      // Find the index of 'C' and move to the next character
      int startIndex = incomingString.indexOf('C') + 8;
      // Extract the two numbers after 'C'
      String clockPosition = incomingString.substring(startIndex, startIndex + 2);
      
      // Print the extracted clock position
      Serial.print("Extracted Clock Position: ");
      Serial.println(clockPosition);

      // Display the clock position on the OLED
      display.clearDisplay(); // Clear the previous display
      drawClockNumber(clockPosition.toInt());
      display.display();
    }

 

void drawClockNumber(int position) {
  // Map the position to the corresponding clock number
  String clockNumber;
  switch (position) {
    case 0: clockNumber = "12"; break;
    case 1: clockNumber = "1"; break;
    case 2: clockNumber = "2"; break;
    case 3: clockNumber = "3"; break;
    case 4: clockNumber = "4"; break;
    case 5: clockNumber = "5"; break;
    case 6: clockNumber = "6"; break;
    case 7: clockNumber = "7"; break;
    case 8: clockNumber = "8"; break;
    case 9: clockNumber = "9"; break;
    case 10: clockNumber = "10"; break;
    case 11: clockNumber = "11"; break;
    default: clockNumber = ""; break;
  }

  // Display the clock number in the center of the screen
  display.setTextSize(5);
  display.setCursor(40, 20);
  display.print(clockNumber);
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
