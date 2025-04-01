#!/bin/bash

# Simple Shell Script to Compile and Upload Arduino Sketch
# Usage: ./upload.sh <sketch.ino> <fqbn> <port>
# Example: ./upload.sh my_sketch.ino arduino:avr:uno /dev/ttyUSB0

set -e  # Exit on error

# Check if the correct number of arguments is provided
if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <sketch.ino> <fqbn> <port>"
    echo "Example: $0 my_sketch.ino arduino:avr:uno /dev/ttyUSB0"
    exit 1
fi

SKETCH=$1
FQBN=$2
PORT=$3

# Compile the sketch
echo "Compiling $SKETCH for $FQBN..."
arduino-cli compile --fqbn "$FQBN" "$SKETCH"

# Upload the compiled sketch
echo "Uploading to $PORT..."
arduino-cli upload -p "$PORT" --fqbn "$FQBN" "$SKETCH"

echo "Upload successful!"
