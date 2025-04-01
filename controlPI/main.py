#!/usr/bin/env python3
import time
import logging
import sys
from motor_controller import MotorController

def run_test_sequence():
    """Run a basic test sequence"""
    logger = logging.getLogger("MotorController")
    logger.info("===== Starting Motor Controller Test =====")
   
    # Create controller - modify I2C bus as needed for your setup
    controller = MotorController(i2c_bus=3, address=0x08)
   
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
    logger = logging.getLogger("MotorController")
    logger.info("===== Starting Interactive Control Mode =====")
   
    # Create controller - modify I2C bus as needed for your setup
    controller = MotorController(i2c_bus=3, address=0x08)
   
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
    # Configure logging
    logging.basicConfig(
        level=logging.DEBUG,
        format='%(asctime)s [%(levelname)s] %(message)s',
        handlers=[
            logging.FileHandler("motor_controller.log"),
            logging.StreamHandler(sys.stdout)
        ]
    )
    
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
