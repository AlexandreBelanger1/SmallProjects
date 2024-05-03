import RPi.GPIO as GPIO
import time
import datetime
from DHT20Script import read_data
#from soil_moisture import read_sensor
import soil_moisture as SM
import pandas as pd

GPIO.setmode(GPIO.BCM)


Fan_Control = 20            # GPIO pin for controlling the heating lamp
Heating_Lamp_Control = 12   # GPIO pin for controlling the heating lamp
Humidifer_Control = 26      # GPIO pin for controlling the humidifier
GPIO.setup(Fan_Control, GPIO.OUT)
GPIO.setup(Heating_Lamp_Control, GPIO.OUT)
GPIO.setup(Humidifer_Control, GPIO.OUT)

high_temp_thres = 24  # Higher Temperature threshold
low_temp_thres = 23   # Lower Temperature threshold
high_humid_thres = 60.0     # Humidity threshold
low_humid_thres = 35.0      # Humidity threshold
lastSoilTemp = 24

try:
    while True:
        temp_air, humidity = read_data()
        try:
            moisture, temp_soil = SM.read_sensor()

        except:
            print("sensor is busy, using previous data point")
            temp_soil = lastSoilTemp
        temp = (temp_air + temp_soil)/2             #taking average of air and soil temperature
        lastSoilTemp = temp_soil
        print("Air Temp", temp_air)
        print(temp)
        print(humidity)

        #Read historical data as dataframes
        df1 = pd.read_csv('data1.csv') # Air Temperature
        df2 = pd.read_csv('data2.csv') # Air Humidity
        df3 = pd.read_csv('data3.csv') # Soil Temperature
        df4 = pd.read_csv('data4.csv') # Soil Moisture
        #Read sensor data as dataframes
        currentTime = datetime.datetime.now()
        df1new = pd.DataFrame({"TimeStamp":[currentTime],"Value":[temp_air]})
        df2new = pd.DataFrame({"TimeStamp":[currentTime],"Value":[humidity]})
        df3new = pd.DataFrame({"TimeStamp":[currentTime],"Value":[temp_soil]})
        df4new = pd.DataFrame({"TimeStamp":[currentTime],"Value":[moisture]})
        #Append sensor data onto historical dataframes
        #df1._append(df1new,ignore_index=False)
        #df2._append(df2new,ignore_index=True)
        #df3._append(df3new,ignore_index=True)
        #df4._append(df4new,ignore_index=True)
        df1.loc[len(df1)] = {"Timestamp":currentTime,"Value":temp_air}
        df2.loc[len(df2)] = {"Timestamp":currentTime,"Value":humidity}
        df3.loc[len(df3)] = {"Timestamp":currentTime,"Value":temp_soil}
        df4.loc[len(df4)] = {"Timestamp":currentTime,"Value":moisture}
        #Write updated dataframes back into CSVs
        df1.to_csv("data1.csv",index=False)
        df2.to_csv("data2.csv",index=False)
        df3.to_csv("data3.csv",index=False)
        df4.to_csv("data4.csv",index=False)




        if (temp != None) and (humidity != None):

            #Logic for controlling temperature
            if (temp > high_temp_thres) or (humidity > high_humid_thres):
                GPIO.output(Fan_Control, GPIO.HIGH)
                print(f"High Temp Detected {temp} C, Fan turned on")
            else:
                GPIO.output(Fan_Control, GPIO.LOW)
            
            if temp < low_temp_thres:
                GPIO.output(Heating_Lamp_Control, GPIO.HIGH)
                print(f"Low Temp Detected {temp} C, Heating Lamp turned on")
            else:
                GPIO.output(Heating_Lamp_Control, GPIO.LOW)

            #Logic for controlling humidity
            # if humidity < low_humid_thres:
            #     GPIO.output(Humidifer_Control, GPIO.HIGH)
            #     print(f"Low Humidity warning {humidity}%, turning on Humidifier")
            # else:
            #     GPIO.output(Humidifer_Control, GPIO.LOW)
        else:
            print("failed to read data from sensor")

        time.sleep(30)       
except KeyboardInterrupt:
    print("Program stopped by user")

finally:
    GPIO.cleanup()
    
