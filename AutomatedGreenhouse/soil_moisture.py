#Latest Version
import board
import time
import RPi.GPIO as GPIO
# import adafruit_seesaw
from adafruit_seesaw.seesaw import Seesaw
# Setup
i2c_bus = board.I2C()
ss = Seesaw(i2c_bus, addr=0x36)

pump_pin = 8  # GPIO pin for the pump
GPIO.setmode(GPIO.BCM)  # Use Broadcom pin-numbering scheme
GPIO.setup(pump_pin, GPIO.OUT)  # Set pump pin to output mode

moisture_threshold = 450  # Tune this value based on testing
pump_on_duration = 5  # Pump will stay on for 5 seconds
last_pump_activation_time = 0  # Time when the pump was last activated

def pump_control(turn_on):
    """Controls the pump based on the turn_on flag."""
    global last_pump_activation_time
    if turn_on:
        GPIO.output(pump_pin, True)  # Turn the pump on
        time.sleep(pump_on_duration)  # Keep the pump on for 5 seconds
        print("Soil is moist, turning pump OFF")
        GPIO.output(pump_pin, False)  # Then turn the pump off
        last_pump_activation_time = time.time()
    else:
        GPIO.output(pump_pin, False)  # Ensure the pump is off

def read_sensor():
    """Reads moisture and temperature from the sensor and returns them."""
   
    moisture = ss.moisture_read()
    temp = ss.get_temp()
    return moisture, temp


def main():
    try:
        while True:
            # Read moisture level and temperature from the sensor
            moisture, temp = read_sensor()
            print(f"temp: {temp}, moisture: {moisture}")

            current_time = time.time()
            # Check if enough time has passed since last activation
            if (current_time - last_pump_activation_time) > pump_on_duration: #change to 1h
                if moisture < moisture_threshold:
                    print("Soil is dry, turning pump ON")
                    pump_control(True)
                else:
                    
                    pump_control(False)


            time.sleep(30)  # Adjust the sleep time as needed for your application

    except KeyboardInterrupt:
        # Clean up GPIO state and exit
        GPIO.cleanup()
        print("Program exited cleanly")

if __name__ == "__main__":
    main()
