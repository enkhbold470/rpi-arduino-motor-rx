// Motor control pins
const int enA = 6;
const int in1 = 4;
const int in2 = 5;
const int in3 = 7;
const int in4 = 8;
const int enB = 9;

// Buzzer and button pins
const int buzzer = A0;
const int startButton = 12;

// Variables
int buttonState = 0;
int lastButtonState = 0;
bool systemActive = false;

void setup() {
  // Initialize motor control pins as outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);
  
  // Initialize buzzer and button
  pinMode(buzzer, OUTPUT);
  pinMode(startButton, INPUT_PULLUP);
  
  // Start serial communication
  Serial.begin(9600);
  
  // Print instructions
  Serial.println("Motor Control System Ready");
  Serial.println("Commands:");
  Serial.println("F - Forward");
  Serial.println("B - Backward");
  Serial.println("L - Turn Left");
  Serial.println("R - Turn Right");
  Serial.println("S - Stop");
  Serial.println("1-9 - Set speed (1=slowest, 9=fastest)");
  Serial.println("X - Toggle system (enable/disable)");
  Serial.println("Press physical button or send 'X' to start system");
  
  // Initial state - motors stopped
  stopMotors();
}

void loop() {
  // Check button state
  buttonState = digitalRead(startButton);
  
  // Detect button press (LOW because of INPUT_PULLUP)
  if (buttonState == LOW && lastButtonState == HIGH) {
    systemActive = !systemActive;
    beep();
    Serial.print("System ");
    Serial.println(systemActive ? "ACTIVATED" : "DEACTIVATED");
    delay(200); // debounce
  }
  lastButtonState = buttonState;
  
  // Process serial commands if available
  if (Serial.available() > 0) {
    char command = Serial.read();
    processCommand(command);
  }
}

void processCommand(char command) {
  command = toupper(command); // Convert to uppercase for case-insensitive handling
  
  if (!systemActive && command != 'X') {
    Serial.println("System is inactive. Press button or send 'X' to activate.");
    return;
  }
  
  switch (command) {
    case 'F': // Forward
      forward();
      Serial.println("Moving FORWARD");
      break;
      
    case 'B': // Backward
      backward();
      Serial.println("Moving BACKWARD");
      break;
      
    case 'L': // Turn Left
      turnLeft();
      Serial.println("Turning LEFT");
      break;
      
    case 'R': // Turn Right
      turnRight();
      Serial.println("Turning RIGHT");
      break;
      
    case 'S': // Stop
      stopMotors();
      Serial.println("STOPPED");
      break;
      
    case '1': // Speed 1 (slowest)
      setSpeed(28); // ~30% of 255
      Serial.println("Speed set to 1 (slow)");
      break;
      
    case '2':
      setSpeed(56); // ~55% of 255
      Serial.println("Speed set to 2");
      break;
      
    case '3':
      setSpeed(85); // ~85% of 255
      Serial.println("Speed set to 3");
      break;
      
    case '4':
      setSpeed(113); // ~113% of 255
      Serial.println("Speed set to 4");
      break;
      
    case '5':
      setSpeed(141); // ~141% of 255
      Serial.println("Speed set to 5 (medium)");
      break;
      
    case '6':
      setSpeed(170); // ~170% of 255
      Serial.println("Speed set to 6");
      break;
      
    case '7':
      setSpeed(198); // ~198% of 255
      Serial.println("Speed set to 7");
      break;
      
    case '8':
      setSpeed(226); // ~226% of 255
      Serial.println("Speed set to 8");
      break;
      
    case '9':
      setSpeed(255); // 100%
      Serial.println("Speed set to 9 (fastest)");
      break;
      
    case 'X': // Toggle system
      systemActive = !systemActive;
      beep();
      Serial.print("System ");
      Serial.println(systemActive ? "ACTIVATED" : "DEACTIVATED");
      if (!systemActive) stopMotors();
      break;
      
    default:
      Serial.println("Unknown command");
      break;
  }
}

// Motor control functions
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

void backward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void forward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void stopMotors() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void setSpeed(int speed) {
  analogWrite(enA, speed);
  analogWrite(enB, speed);
}

void beep() {
  tone(buzzer, 1000, 200);
}