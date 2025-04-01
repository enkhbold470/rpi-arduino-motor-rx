import serial
import time
import logging
import sys

# Configure logging to show on console and save to file
logging.basicConfig(
    level=logging.DEBUG,
    format='%(asctime)s [%(levelname)s] %(message)s',
    handlers=[
        logging.FileHandler("motor_controller.log"),
        logging.StreamHandler(sys.stdout)
    ]
)
logger = logging.getLogger("MotorController")

class MotorController:
    """Simple motor controller with clear logging for Arduino communication"""
   
    def __init__(self, port='/dev/ttyACM0', baudrate=9600):  # Modified port
        self.port = port
        self.baudrate = baudrate
        self.ser = None
        logger.info(f"Initializing MotorController on {port} at {baudrate} baud")
        self.connect()
       
    def connect(self):
        """Establish serial connection with the Arduino"""
        try:
            logger.info(f"Opening serial port {self.port}")
            self.ser = serial.Serial(
                port=self.port,
                baudrate=self.baudrate,
                timeout=5  # Increased timeout
            )
           
            # Wait for Arduino to reset after serial connection
            logger.info("Waiting for Arduino initialization")
            start_time = time.time()
            while time.time() - start_time < 5: # Wait up to 5 seconds
                if self.ser.in_waiting > 0:
                    line = self.ser.readline().decode().strip()
                    logger.info(f"Received: {line}")
                    if "BOOT:READY" in line:
                        logger.info("Arduino is ready!")
                        return True
                time.sleep(0.1)
           
            logger.error("Arduino did not send BOOT:READY. Connection failed.")
            self.ser.close()
            self.ser = None
            return False
   
        except Exception as e:
            logger.error(f"Connection error: {str(e)}")
            return False
   
    def send_command(self, cmd, wait_for_response=True):
        """Send a single character command to the Arduino"""
        if not self.ser or not self.ser.is_open:
            logger.error("Cannot send command - serial port not open")
            return "ERROR: Serial port not open"
           
        # Send single character command with newline
        try:
            logger.debug(f"Sending command: '{cmd}'")
            self.ser.write(f"{cmd}\n".encode())
           
            if wait_for_response:
                # Wait for response (up to 1 second)
                time.sleep(0.1)  # Give Arduino time to process
               
                # Read response
                if self.ser.in_waiting > 0:
                    response = self.ser.readline().decode().strip()
                    logger.debug(f"Response: '{response}'")
                    return response
                else:
                    logger.warning(f"No response for command '{cmd}'")
                    return "ERROR: No response"
            else:
                return "Command sent"
               
        except Exception as e:
            logger.error(f"Error sending command: {str(e)}")
            return f"ERROR: {str(e)}"
   
    def test_communication(self):
        """Test basic communication with the Arduino"""
        logger.info("Testing communication...")
       
        # Request status
        response = self.send_command('?')
        logger.info(f"Status response: {response}")
       
        if "STAT" in response:
            logger.info("Communication test passed!")
            return True
        else:
            logger.error("Communication test failed!")
            return False
   
    # Movement commands
    def forward(self):
        """Move forward"""
        logger.info("Moving forward")
        return self.send_command('F')
   
    def backward(self):
        """Move backward"""
        logger.info("Moving backward")
        return self.send_command('B')
   
    def left(self):
        """Turn left"""
        logger.info("Turning left")
        return self.send_command('L')
   
    def right(self):
        """Turn right"""
        logger.info("Turning right")
        return self.send_command('R')
   
    def stop(self):
        """Stop all motors"""
        logger.info("Stopping motors")
        return self.send_command('S')
   
    def set_speed(self, level):
        """Set speed level (0-9)"""
        if 0 <= level <= 9:
            logger.info(f"Setting speed to level {level}")
            return self.send_command(str(level))
        else:
            logger.error(f"Invalid speed level: {level} (must be 0-9)")
            return "ERROR: Invalid speed level (must be 0-9)"
   
    def toggle_system(self):
        """Toggle system state (on/off)"""
        logger.info("Toggling system state")
        return self.send_command('X')
   
    def get_status(self):
        """Get system status"""
        logger.info("Requesting system status")
        return self.send_command('?')
   
    def close(self):
        """Close serial connection"""
        if self.ser and self.ser.is_open:
            logger.info("Closing serial connection")
            self.ser.close()
            return True
        return False

def run_test_sequence():
    """Run a basic test sequence"""
    logger.info("===== Starting Motor Controller Test =====")
   
    # Create controller - change port to match your setup
    # Common options:
    # - Orange Pi USB: '/dev/ttyACM0'
    # - Orange Pi GPIO UART: '/dev/ttyS0'
    controller = MotorController('/dev/ttyACM0', baudrate=9600) # Modified port
   
    try:
        # Test communication
        if not controller.test_communication():
            logger.error("Failed to communicate with Arduino")
            return
           
        # Turn on the system
        logger.info("Enabling system")
        controller.toggle_system()
        time.sleep(0.5)
       
        # Test movement
        logger.info("Testing movement commands")
       
        controller.set_speed(5)  # Set medium speed
        time.sleep(0.5)
       
        controller.forward()
        time.sleep(1)
       
        controller.right()
        time.sleep(1)
       
        controller.backward()
        time.sleep(1)
       
        controller.left()
        time.sleep(1)
       
        controller.stop()
        time.sleep(0.5)
       
        # Turn off the system
        logger.info("Disabling system")
        controller.toggle_system()
       
    except Exception as e:
        logger.error(f"Error during testing: {str(e)}")
    finally:
        # Stop motors and close connection
        controller.stop()
        controller.close()
        logger.info("===== Test Complete =====")

def interactive_mode():
    """Start an interactive control mode"""
    logger.info("===== Starting Interactive Control Mode =====")
   
    # Create controller - change port to match your setup
    controller = MotorController('/dev/ttyACM0', baudrate=9600) # Modified Port
   
    # Test communication
    if not controller.test_communication():
        logger.error("Failed to communicate with Arduino")
        return
   
    print("\nInteractive Control Mode")
    print("------------------------")
    print("Commands:")
    print("  F - Move Forward")
    print("  B - Move Backward")
    print("  L - Turn Left")
    print("  R - Turn Right")
    print("  S - Stop")
    print("  0-9 - Set Speed")
    print("  X - Toggle System On/Off")
    print("  ? - Get Status")
    print("  Q - Quit")
    print("")
   
    try:
        while True:
            cmd = input("Enter command: ").strip().upper()
           
            if cmd == 'Q':
                break
               
            if cmd in ['F', 'B', 'L', 'R', 'S', 'X', '?'] or cmd.isdigit():
                response = controller.send_command(cmd)
                print(f"Response: {response}")
            else:
                print("Invalid command")
               
    except KeyboardInterrupt:
        print("\nExiting...")
    finally:
        # Stop motors and close connection
        controller.stop()
        controller.close()
        logger.info("===== Interactive Mode Ended =====")

if __name__ == "__main__":
    print("\nArduino Motor Controller")
    print("1. Run test sequence")
    print("2. Start interactive mode")
    choice = input("Select an option (1/2): ").strip()
   
    if choice == '1':
        run_test_sequence()
    elif choice == '2':
        interactive_mode()
    else:
        print("Invalid choice")
