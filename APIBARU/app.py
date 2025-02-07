from flask import Flask, render_template, request
import serial
import serial.tools.list_ports
import time

app = Flask(__name__)

# Function to find Arduino's serial port
def find_arduino_port():
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        if "Arduino" in p.description:
            return p.device
    return None

# Find the Arduino's port
arduino_port = find_arduino_port()

# Configure serial port
ser = None  # Initialize ser to None
if arduino_port:
    try:
        ser = serial.Serial(arduino_port, 9600, timeout=1)
        time.sleep(2)  # Wait for connection to establish
        print(f"Serial port {arduino_port} opened successfully.")
    except serial.SerialException as e:
        print(f"Error opening serial port {arduino_port}: {e}")
else:
    print("Arduino not found. Please ensure it is connected.")

# Global variables
led_state = "OFF"
servo_position = 90

@app.route("/")
def index():
    return render_template('index.html', led_state=led_state, servo_position=servo_position)

@app.route("/led/<action>")
def led_control(action):
    global led_state
    if ser and ser.is_open:
        try:
            if action == "on":
                ser.write(b"ledOn\n")
                led_state = "ON"
            elif action == "off":
                ser.write(b"ledOff\n")
                led_state = "OFF"
            return "OK"
        except serial.SerialException as e:
            print(f"Serial communication error: {e}")
            return "Error: Serial communication error"
    else:
        return "Error: Serial port not available or not open"

@app.route("/servo", methods=['POST'])
def servo_control():
    global servo_position
    if ser and ser.is_open:
        position = request.form['position']
        try:
            position_int = int(position)
            if 0 <= position_int <= 180:
                ser.write(f"servo{position_int}\n".encode())
                servo_position = position_int
                return "OK"
            else:
                return "Error: Invalid servo position"
        except ValueError:
            return "Error: Invalid servo position"
        except serial.SerialException as e:
            print(f"Serial communication error: {e}")
            return "Error: Serial communication error"
    else:
        return "Error: Serial port not available or not open"

# Properly close the serial port when the app shuts down
def close_serial_port():
    if ser and ser.is_open:
        ser.close()
        print("Serial port closed.")

import atexit
atexit.register(close_serial_port)

if __name__ == '__main__':
    app.run(debug=True, use_reloader=False)
