#include <DHT.h>
#include <DHT_U.h>

#include <DHT.h>
#include <DHT_U.h>

#include <dht.h>


// Use the Adafruit DHT Sensor Library
// (this also requires the Adafruit Unified Sensor Library)
#include "DHT.h"


#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Arduino pin connected to the DHT-11's data pin.
#define DHTPIN 9

// Type of DHT sensor.
#define DHTTYPE DHT11

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

void setup() {

  lcd.begin(16, 2);
  // Start the sensor.
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
    float h = dht.readHumidity();

  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  lcd.setCursor(0,0); 
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.print("%");
  delay(1000);
  // Read humidity.


}
