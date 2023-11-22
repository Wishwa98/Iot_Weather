#include <DHT.h>

//Constants
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11  (AM2302)
// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);

//Variables
int chk;
float humFloat;  //Stores humidity value
float tempFloat; //Stores temperature value

void setup()
{
    Serial.begin(9600);
  dht.begin();
    
}

String humStr;
String tempStr;

void loop()
{
    //Read data and store it to variables hum and temp
    humFloat = dht.readHumidity();
    tempFloat= dht.readTemperature();
    humStr = String(humFloat);
    tempStr = String(tempFloat);
    //Print temp and humidity values to serial monitor
   Serial.print("Humidity: ");
    Serial.print(tempStr);
    Serial.print(" , ");
    Serial.println(tempStr);
   Serial.println(" Celsius");
    
    delay(2000); //Delay 2 sec.
}
