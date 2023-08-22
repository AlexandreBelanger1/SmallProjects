
// Use the Adafruit DHT Sensor Library
// (this also requires the Adafruit Unified Sensor Library)
#include "DHT.h"

// Arduino pin connected to the DHT-11's data pin.
#define DHTPIN 2

// Type of DHT sensor.
#define DHTTYPE DHT11

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // We'll use the serial monitor to display readings.
  Serial.begin(9600);
  Serial.println("Humidity and Temperature");

  // Start the sensor.
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Read humidity.
  float h = dht.readHumidity();

  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Send data to serial monitor for display.
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(t);
  Serial.println("°C");
}
