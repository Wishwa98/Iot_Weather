//importing the needed library
#include "DHT.h"

//defining the dataoutput pin
#define DHTPIN 2

//defining the sensor type
#define DHTTYPE DHT11

//creating and object and pasing the defined variables
DHT dht(DHTPIN,DHTTYPE);

void setup() {

//displaying the outputs in Serial Monitor  
Serial.begin(9600);
Serial.println("Humditiy and Temperature");

dht.begin();
}

void loop() {
  
delay(2000);

//Reading the humidity and temperatures
float Humidity = dht.readHumidity();
float TempC = dht.readTemperature();



Serial.print(Humidity);
Serial.print(",");

Serial.println(TempC);

}
