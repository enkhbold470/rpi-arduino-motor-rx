#include <Wire.h>

#define SLAVE_ADDRESS 0x08 // Define the I2C address for this Arduino

volatile byte receivedData = 0; // Variable to store received data
volatile bool dataReceived = false; // Flag to indicate new data

void setup() {
  Wire.begin(SLAVE_ADDRESS);     // Join I2C bus with the defined address
  Wire.onReceive(receiveEvent); // Register function to call when data received
  // Wire.onRequest(requestEvent); // Optional: Register function for sending data back

  Serial.begin(9600);          // Start serial for debugging output
  Serial.println("Arduino I2C Slave Ready!");
  Serial.print("Address: 0x");
  Serial.println(SLAVE_ADDRESS, HEX);
}

void loop() {
  // You can add other non-I2C tasks here if needed
  delay(100); // Small delay
}

// Function that executes whenever data is received from master
void receiveEvent(int howMany) {
  if (Wire.available()) { // Check if there's data waiting
    receivedData = Wire.read(); // Read the byte
    dataReceived = true;        // Set the flag

    // Print to Serial Monitor for debugging
    Serial.print("Received: ");
    Serial.println(receivedData);
  }
}

/*
// Optional: Function that executes when master requests data
void requestEvent() {
  // Example: Send back the last received value + 1
  byte response = 0;
  if (dataReceived) {
      response = receivedData + 1;
      dataReceived = false; // Clear the flag after responding
  }
  Wire.write(response); // Send response byte back to master
  Serial.print("Sent response: ");
  Serial.println(response);
}
*/
