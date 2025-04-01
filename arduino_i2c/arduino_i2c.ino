// --- START OF CORRECT SLAVE CODE ---
#include <Wire.h>

#define SLAVE_ADDRESS 0x08 // Arduino's address

volatile byte receivedData = 0;

void setup() {
  Wire.begin(SLAVE_ADDRESS);     // Join I2C bus as SLAVE at 0x08
  Wire.onReceive(receiveEvent); // Function to call when master sends data
  Serial.begin(9600);
  Serial.println("Arduino I2C Slave Ready!");
  Serial.print("Address: 0x");
  Serial.println(SLAVE_ADDRESS, HEX);
}

void loop() {
  delay(100);
}

void receiveEvent(int howMany) {
  if (Wire.available()) {
    receivedData = Wire.read();
    Serial.print("Received: ");
    Serial.println(receivedData);
  }
}
// --- END OF CORRECT SLAVE CODE ---