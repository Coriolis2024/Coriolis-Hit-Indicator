const int mosfetPin = 3; // The MOSFET is connected to digital pin 3

void setup() {
  // Initialize the MOSFET pin as an output
  pinMode(mosfetPin, OUTPUT);
  // Begin serial communication at a baud rate of 9600
  Serial.begin(9600);
}

void loop() {
  // Check if there's any data available on the serial port
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
  
