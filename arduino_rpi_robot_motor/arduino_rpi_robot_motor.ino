#include <SoftwareSerial.h>

// Motor control pins
const int enA = 6;  // Enable motor A
const int in1 = 4;  // Motor A input 1
const int in2 = 5;  // Motor A input 2
const int in3 = 7;  // Motor B input 1
const int in4 = 8;  // Motor B input 2
const int enB = 9;  // Enable motor B

// Buzzer and button pins
const int buzzer = A0;
const int startButton = 12;

// Software Serial (RX, TX)
SoftwareSerial orangePiSerial(A1, A2); // RX = A1, TX = A2

// Variables
bool systemActive = false;    // System active flag
int currentSpeed = 200;       // Current motor speed (0-255)
unsigned long lastActivityTime = 0;
const unsigned long WATCHDOG_TIMEOUT = 15000;  // 15 second timeout

void setup() {
  // Initialize motor control pins
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);
  
  // Initialize buzzer and button
  pinMode(buzzer, OUTPUT);
  pinMode(startButton, INPUT_PULLUP);
  
  // Set initial motor speed
  analogWrite(enA, currentSpeed);
  analogWrite(enB, currentSpeed);
  
  // Start hardware serial (for debugging)
  Serial.begin(9600);
  
  // Start software serial for Orange Pi communication
  orangePiSerial.begin(9600);
  
  // Initial state
  stopMotors();
  
  // Short beep to indicate boot
  beep(100);  
  delay(100);
  
  Serial.println("BOOT:READY");
  orangePiSerial.println("BOOT:READY");

  // Double beep to indicate ready
  beep(100);
  delay(100);
  beep(100);
  
  lastActivityTime = millis();
}

void loop() {
  // Check button state for manual toggle
  if (digitalRead(startButton) == LOW) {
    toggleSystem();
    beep(150);  // Feedback beep
    delay(200);  // Longer debounce
  }

  // Process serial commands
  processSerialCommands();
  
  // Check for watchdog timeout
  if (systemActive && millis() - lastActivityTime > WATCHDOG_TIMEOUT) {
    Serial.println("WATCHDOG: Timeout - stopping motors");
    stopMotors();
    systemActive = false;
    beep(800);  // Long warning beep
  }
}

void processSerialCommands() {
  // Process from Orange Pi serial
  while (orangePiSerial.available() > 0) {
    char c = orangePiSerial.read();
    
    // Process single character commands immediately
    if (c == 'F' || c == 'B' || c == 'L' || c == 'R' || c == 'S' || 
        c == 'X' || c == '?' || (c >= '0' && c <= '9')) {
      
      processCommand(c);
      
      // Consume any trailing newlines or carriage returns
      while (orangePiSerial.available() > 0) {
        char next = orangePiSerial.peek();
        if (next == '\n' || next == '\r') {
          orangePiSerial.read(); // Consume it
        } else {
          break;
        }
      }
    }
  }
  
  // Process from debug serial
  while (Serial.available() > 0) {
    char c = Serial.read();
    
    // Process single character commands immediately
    if (c == 'F' || c == 'B' || c == 'L' || c == 'R' || c == 'S' || 
        c == 'X' || c == '?' || (c >= '0' && c <= '9')) {
      
      processCommand(c);
      
      // Consume any trailing newlines or carriage returns
      while (Serial.available() > 0) {
        char next = Serial.peek();
        if (next == '\n' || next == '\r') {
          Serial.read(); // Consume it
        } else {
          break;
        }
      }
    }
  }
}

void processCommand(char command) {
  lastActivityTime = millis();  // Reset watchdog timer
  String response;
  
  // Echo command for verification
  Serial.print("CMD: ");
  Serial.println(command);
  
  // Process based on command character
  switch (command) {
    case 'F': // Forward
      if (systemActive) {
        forward();
        response = "ACK:FWD";
      } else {
        response = "ERR:INACTIVE";
      }
      break;
      
    case 'B': // Backward
      if (systemActive) {
        backward();
        response = "ACK:BWD";
      } else {
        response = "ERR:INACTIVE";
      }
      break;
      
    case 'L': // Left
      if (systemActive) {
        turnLeft();
        response = "ACK:LEFT";
      } else {
        response = "ERR:INACTIVE";
      }
      break;
      
    case 'R': // Right
      if (systemActive) {
        turnRight();
        response = "ACK:RIGHT";
      } else {
        response = "ERR:INACTIVE";
      }
      break;
      
    case 'S': // Stop
      stopMotors();
      response = "ACK:STOP";
      break;
      
    case '0'...'9': // Speed
      int speedLevel = command - '0';
      setSpeed(map(speedLevel, 0, 9, 50, 255));  // Map 0-9 to 50-255
      response = "ACK:SPD:" + String(speedLevel);
      break;
      
    case 'X': // Toggle system
      toggleSystem();
      response = "ACK:SYS:" + String(systemActive ? "ON" : "OFF");
      break;
      
    case '?': // Status
      response = "STAT:" + String(systemActive ? "ON" : "OFF") + 
                ":SPD:" + String(map(currentSpeed, 50, 255, 0, 9));
      break;
      
    default:
      response = "ERR:INVALID";
      break;
  }
  
  // Send response back to both serial interfaces
  Serial.println(response);
  orangePiSerial.println(response);
}

void toggleSystem() {
  systemActive = !systemActive;
  beep(systemActive ? 200 : 400);  // Different tones for on/off
  
  if (!systemActive) {
    stopMotors();  // Safety: stop motors when system is turned off
  }
  
  lastActivityTime = millis();  // Reset watchdog timer
}

// Motor control functions
void forward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void backward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnLeft() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnRight() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void stopMotors() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void setSpeed(int speed) {
  currentSpeed = constrain(speed, 0, 255);
  analogWrite(enA, currentSpeed);
  analogWrite(enB, currentSpeed);
}

void beep(int duration) {
  tone(buzzer, 1000, duration);
}