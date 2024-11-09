#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

// Define the pins for the SoftwareSerial connection
const int RXPin = 4; // Connect to TX of GT-U7
const int TXPin = 3; // Connect to RX of GT-U7
const uint32_t GPSBaud = 9600; // Baud rate for the GPS module

// Create a TinyGPS++ object
TinyGPSPlus gps;

// Create a SoftwareSerial object
SoftwareSerial gpsSerial(RXPin, TXPin);
String lora_RX_address = "2";

void setup() {
  // Start the serial communication with the computer
  Serial.begin(9600);
  // Start the serial communication with the GPS module
  gpsSerial.begin(GPSBaud);
  Serial.println("GPS tracking started");
}

void loop() {
  // Check if data is available from the GPS module
  while (gpsSerial.available() > 0) {
    // Read the data from the GPS module
    gps.encode(gpsSerial.read());
  }

  // If a valid location is obtained, print it to the Serial Monitor
  if (gps.location.isUpdated()) {
    
    
    String latitude = String(gps.location.lat(), 6);
    String longitude = String(gps.location.lng(), 6);
   
    
    String loraCommandlat = ("AT+SEND=2,20,") + (latitude) + (",") + (longitude);
    
    Serial.println(loraCommandlat);
    delay(5000);
    
   
  }

 
  
}
