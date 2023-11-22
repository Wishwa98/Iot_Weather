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
  myservo.attach(5); //D1
}

void loop() {
  delay(1000);
  //reading the data from the sensor
  float temp = dht.readTemperature();

  //converting the gathered temp to farenhite

  float temp_f = temp*1.8+32;

  float hum = dht.readHumidity();

  //gathering the heatIndex
  float heatIndex = calculateHeatIndex(temp_f, hum);
  
  // Convert the heat index to degrees for the servo motor
  int degrees = convertToDegrees(heatIndex);
 
  // Map the degrees to the servo motor position
  int servoPosition = map(degrees, 0, 180, 0, 180);

  // Move the servo motor to the calculated position
  myservo.write(servoPosition);

  delay(1000);
 
  if (isnan(hum) || isnan(temp_f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  //printing the values on serial monitor
  Serial.print("Temperature: ");
  Serial.print(temp_f);
  Serial.print(" Humidity: ");
  Serial.print(hum);
  Serial.print(" Heat Index: ");
  Serial.println(heatIndex);

    // Display the heat index on the scale
  if (heatIndex >= 0 && heatIndex <= 80) {
    Serial.println("Normal");
  } else if (heatIndex > 80 && heatIndex <= 90) {
    Serial.println("Caution");
  } else if (heatIndex > 90 && heatIndex <= 103) {
    Serial.println("Extreme Caution");
  } else if (heatIndex > 103 && heatIndex <= 124) {
    Serial.println("Danger");
  } else if (heatIndex > 124) {
    Serial.println("Extreme Danger");
  }

   //Publish The Sensor Data
  char tempString[8];
  char humString[8];
  dtostrf(temp_f, 6, 2, tempString);
  dtostrf(hum, 6, 2, humString);
  client.connect("ESP8266Client");
  client.publish("dht11/temperature", tempString);
  client.publish("dht11/humidity", humString);
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

float calculateHeatIndex(float temp_f, float hum) {
  float heatIndex = 0;

  // calculate the heat index using the formula provided by the National Weather Service

  // Calculate the heat index using the formula
  heatIndex = -42.379 + 2.04901523*temp_f + 10.14333127*hum - 0.22475541*temp_f*hum - 0.00683783*temp_f*temp_f - 0.05481717*hum*hum + 0.00122874*temp_f*temp_f*hum + 0.00085282*temp_f*hum*hum - 0.00000199*temp_f*temp_f*hum*hum;

  return heatIndex;
}

//map(value, fromLow, fromHigh, toLow, toHigh);
int convertToDegrees(float heatIndex) {
  int degrees = 0;
  if (heatIndex >= 0 && heatIndex <= 80) {
    degrees = map(heatIndex, 0, 80, 18, 36);
  } else if (heatIndex > 80 && heatIndex <= 90) {
    degrees = map(heatIndex, 81, 90, 37, 72);
  } else if (heatIndex > 90 && heatIndex <= 103) {
    degrees = map(heatIndex, 91, 103, 73, 108);
  } else if (heatIndex > 103 && heatIndex <= 124) {
    degrees = map(heatIndex, 104, 124, 109, 144);
  } else if (heatIndex > 124) {
    degrees = map(heatIndex, 125, 200, 145, 180);
  }
  return degrees;
}
