import smbus2 # Use smbus2 for better error handling (install with: sudo pip3 install smbus2)
# import smbus # Or use the older smbus if smbus2 is not available
import time

# --- Configuration ---
# IMPORTANT: Change this based on 'ls /sys/devices/platform/soc*/*/i2c-* | grep "i2c-[0-9]"'
# For TWI1 (I2C1) at hardware address 5002400, your output showed i2c-2
I2C_BUS = 3
ARDUINO_ADDRESS = 0x08 # The I2C address of the Arduino (must match Arduino code)
# --- End Configuration ---

# Initialize I2C bus
try:
    bus = smbus2.SMBus(I2C_BUS)
    # bus = smbus.SMBus(I2C_BUS) # Alternative for older smbus library
    print(f"I2C Bus {I2C_BUS} opened successfully.")
except FileNotFoundError:
    print(f"ERROR: I2C Bus {I2C_BUS} not found. Check if enabled in orangepi-config/overlays.")
    exit(1)
except Exception as e:
    print(f"ERROR: Could not open I2C bus {I2C_BUS}: {e}")
    exit(1)

# Simple loop to send incrementing numbers
data_to_send = 0
while True:
    try:
        print(f"Sending: {data_to_send}")
        # Write a single byte to the Arduino
        bus.write_byte(ARDUINO_ADDRESS, data_to_send)

        # Optional: Read a byte back from Arduino (if requestEvent is enabled on Arduino)
        # time.sleep(0.1) # Short delay before reading
        # received_byte = bus.read_byte(ARDUINO_ADDRESS)
        # print(f"Received back: {received_byte}")

        data_to_send = (data_to_send + 1) % 256 # Increment and wrap around 0-255

        time.sleep(1) # Wait 1 second before sending the next byte

    except OSError as e:
        print(f"I2C Error: {e}. Check wiring and device address.")
        print("Is the Arduino powered and connected correctly?")
        time.sleep(2) # Wait before retrying
    except KeyboardInterrupt:
        print("\nExiting.")
        break
    except Exception as e:
        print(f"An unexpected error occurred: {e}")
        break

# Close the bus (optional for simple scripts, but good practice)
try:
    bus.close()
    print("I2C Bus closed.")
except NameError:
    pass # Bus was likely never opened successfully
