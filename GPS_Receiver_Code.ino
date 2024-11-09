#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define the pins for the SoftwareSerial connection
const int RXPin = 4; // Connect to TX of GT-U7
const int TXPin = 3; // Connect to RX of GT-U7
const uint32_t GPSBaud = 9600; // Baud rate for the GPS module

// Create a TinyGPS++ object
TinyGPSPlus gps;

// Create a SoftwareSerial object
SoftwareSerial gpsSerial(RXPin, TXPin);

const double EARTH_RADIUS_KM = 6371.0; // Earth's radius in kilometers
bool newData = false;

// OLED display settings
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Function to convert degrees to radians
double toRadians(double degrees) {
  return degrees * PI / 180.0;
}

// Function to calculate the distance using the Haversine formula
double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
  double dlat = toRadians(lat2 - lat1);
  double dlon = toRadians(lon2 - lon1);

  double a = sin(dlat / 2) * sin(dlat / 2) +
             cos(toRadians(lat1)) * cos(toRadians(lat2)) *
             sin(dlon / 2) * sin(dlon / 2);

  double c = 2 * atan2(sqrt(a), sqrt(1 - a));
  double distance = EARTH_RADIUS_KM * c;

  return distance;
}

void gps_data_update() {
  for (unsigned long start = millis(); millis() - start < 1000;) {
    while (gpsSerial.available()) {
      char c = gpsSerial.read();
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }
}

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 baud rate
  gpsSerial.begin(GPSBaud); // Start the serial communication with the GPS module
  Serial.println(F("GPS tracking started"));

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  gps_data_update(); // Update GPS data

  if (newData) {
    double lat1 = gps.location.lat(); // Latitude of point 1
    double lon1 = gps.location.lng(); // Longitude of point 1

    // Simulate receiving the second set of coordinates
    double lat2 = 0.0; // Initialize latitudeA
    double lon2 = 0.0; // Initialize longitudeA

    // Parse the received data for lat2 and lon2
    if (Serial.available()) {
      String line = Serial.readStringUntil('\n'); // Read until newline character
      if (line.startsWith("+RCV=")) { // Check if the line starts with "+RCV="
        String receivedData = line;
        int firstComma = receivedData.indexOf(',');
        int secondComma = receivedData.indexOf(',', firstComma + 1);
        int thirdComma = receivedData.indexOf(',', secondComma + 1);

        if (firstComma != -1 && secondComma != -1 && thirdComma != -1) {
          lat2 = receivedData.substring(secondComma + 1, thirdComma).toDouble();
          lon2 = receivedData.substring(thirdComma + 1).toDouble();
        }
      }
    }

    // Calculate the distance using the Haversine formula
    double distanceKm = calculateDistance(lat1, lon1, lat2, lon2);
    double distanceYards = distanceKm * 1093.61; // Convert km to yards

    if (distanceYards < 5000) {
      Serial.print(F("Distance: "));
      Serial.print(distanceYards);
      Serial.println(F(" yards"));

      // Display the distance on the OLED screen
      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.print(F("Dist: "));
      display.print(distanceYards);
      display.println(F(" yd"));
      display.display();
    }

    newData = false; // Reset newData flag
  }

  delay(1000); // Delay for 2 seconds before the next update
  
}
