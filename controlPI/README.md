# ControlPI

A Python package for controlling Arduino-based motor robots over I2C communication.

## Installation

```bash
pip install -r requirements.txt
```

## Hardware Setup

1. Connect your Raspberry Pi/Orange Pi to Arduino via I2C:
   - Pi SDA → Arduino A4 (SDA)
   - Pi SCL → Arduino A5 (SCL)
   - Pi GND → Arduino GND

2. Make sure the Arduino is running the provided sketch with I2C slave address set to 0x08.

3. Enable I2C on your Raspberry Pi:
   ```bash
   sudo raspi-config
   ```
   Navigate to Interfacing Options → I2C → Enable

   For Orange Pi, use `sudo armbian-config` or appropriate method.

## Using the package

Basic usage example:

```python
from controlPI import MotorController

# Create a controller, specifying I2C bus (default: 3) and address (default: 0x08)
controller = MotorController(i2c_bus=3, address=0x08)

# Test communication with Arduino
if controller.test_communication():
    # Turn on the system
    controller.toggle_system()
    
    # Set speed (0-9)
    controller.set_speed(5)
    
    # Move the robot
    controller.forward()
    # ... other commands ...
    
    # Stop motors
    controller.stop()
    
    # Clean up when done
    controller.close()
```

## Available Commands

- `forward()` - Move robot forward
- `backward()` - Move robot backward
- `left()` - Turn robot left
- `right()` - Turn robot right
- `stop()` - Stop all motors
- `set_speed(level)` - Set speed level (0-9)
- `toggle_system()` - Turn system on/off
- `get_status()` - Get system status

## Running the demo

The package includes a demo script to test your setup:

```bash
python main.py
```

Select option 1 for a test sequence or option 2 for interactive control.

## Troubleshooting

- Check I2C connection with `i2cdetect -y 3` (use your bus number)
- Ensure Arduino has the correct I2C address (0x08)
- Verify power supply is adequate for motors
- Check log file `motor_controller.log` for debugging information 