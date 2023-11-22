//including the needed libraries
#include <DHT.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//adding a servo motor library
#include <Servo.h>


//Defining the output pin
#define DHTPIN D7

//defining the sensory type
#define DHTTYPE DHT11

//#define mqtt_server "iot.eclipse.org"

//Initilize Wifi Credentials
const char* ssid = "SLT FIBER";
const char* password = "wishwa123";

//public broker has used to publish the readings
const char* mqtt_server = "broker.hivemq.com";

//initializing the server,wificlient ,DHT pin and DHT type
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);


//creating an object for the servo
Servo myservo;

void setup() {
  Serial.begin(9600);
  delay(10);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  myservo.attach(5,544,2400); //D1
}

void loop() {
  delay(1000);
  //reading the data from the sensor
  float temp = dht.readTemperature(true);


  float celcius = dht.readTemperature();

  float hum = dht.readHumidity();
 
  if (isnan(hum) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  float heatIndex = -42.379 + 2.04901523*temp + 10.14333127*hum - .22475541*temp*hum - .00683783*temp*temp - .05481717*hum*hum + .00122874*temp*temp*hum + .00085282*temp*hum*hum - .00000199*temp*temp*hum*hum;

  // Compute heat index in Fahrenheit (the default)
  //float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  //float hic = dht.computeHeatIndex(t, h, false);

  //printing the values on serial monitor
  Serial.print("Temperature: ");
  Serial.print(celcius);
  Serial.print(" Humidity: ");
  Serial.print(hum);
  Serial.print(" Heat Index: ");
  Serial.println(heatIndex);

  //initializng a string to get the heat category
  String heatCategory;

  

    // Display the heat category according to the heat index
  if (heatIndex >= 0 && heatIndex <= 80) {
    heatCategory = "Normal";
  } else if (heatIndex > 80 && heatIndex <= 90) {
    heatCategory = "Caution";
  } else if (heatIndex > 90 && heatIndex <= 103) {
    heatCategory = "Extreme Caution";
  } else if (heatIndex > 103 && heatIndex <= 124) {
    heatCategory = "Danger";
  } else if (heatIndex > 124) {
    heatCategory = "Extreme Danger";
  }

  Serial.println(heatCategory);

  //using a if else condition rotating the servo meter to different agnles according to heat index category
  int x = 0 ;
  if(heatIndex < 80){
      myservo.write(180-18);
      delay(20);
  }
  else if(heatIndex<90){
      myservo.write(180-54);
      delay(20);   
    }
  else if(heatIndex<103){
      myservo.write(180-90);
      delay(20);
    }
  else if(heatIndex<124){
      myservo.write(180-126);
      delay(20);
    }
  else{
      myservo.write(180-175);
      delay(20);
       }

  delay(3000);



  // Length (with one extra character for the null terminator)
  int heat_len = heatCategory.length() + 1; 
  // Prepare the character array (the buffer) 
  char char_heat[heat_len];
  heatCategory.toCharArray(char_heat, heat_len);
  

   //Publish The Sensor Data
  char tempString[8];
  char humString[8];
  char heatString[8];
  char celString[8];
  dtostrf(temp, 6, 2, tempString);
  dtostrf(hum, 6, 2, humString);
  dtostrf(heatIndex, 6, 2, heatString);
  dtostrf(celcius, 6, 2, celString);
  client.connect("ESP8266Client");
  client.publish("dht11/temperature", tempString);
  client.publish("dht11/humidity", humString);
  client.publish("dht11/heatindex", heatString);
  client.publish("dht11/celcius", celString);
  client.publish("dht11/heatCategory", char_heat);
  client.disconnect();





}

//Wifi setup
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

