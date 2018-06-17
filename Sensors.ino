#include <Servo.h>
#include <dht.h>
#include <ArduinoJson.h>

DHT sensor = DHT();
Servo servo;
#define FAN 2
#define LED 13

void setup() {
  Serial.begin(9600);
  while (!Serial) continue;
  pinMode(FAN, OUTPUT);
  pinMode(LED, OUTPUT);
  servo.attach(9);
  
  sensor.attach(A0);
}

int Temperature = 0, Moisture = 0, Humidity = 0, Fan;

void loop() { 
  sensor.update();
  
  Temperature = sensor.getTemperatureInt();
  Humidity = sensor.getHumidityInt();
  Moisture = analogRead(A1);
  StaticJsonBuffer<200> jsonBuffer;

  JsonObject& root = jsonBuffer.createObject();
  JsonObject& sensor = jsonBuffer.createObject();
  JsonObject& devices = jsonBuffer.createObject();
  JsonObject& moisture = jsonBuffer.createObject();
  JsonObject& temperature = jsonBuffer.createObject();
  JsonObject& humidity = jsonBuffer.createObject();

  moisture["value"] = Moisture;
  moisture["name"] = "Moisture";
  if (Moisture <= 0) moisture["State"] = "Not working";
  else moisture["State"] = "Working";
  
  sensor["Moisture"] = moisture;
  
  temperature["value"] = Temperature;
  temperature["name"] = "Temperature";
  if (Temperature <= 0) temperature["State"] = "Not working";
  else temperature["State"] = "Working";
  
  sensor["Temperature"] = temperature;
  
  humidity["value"] = Humidity;
  humidity["name"] = "Humidity";
  if (Humidity <= 0) humidity["State"] = "Not working";
  else humidity["State"] = "Working";
  
  sensor["Humidity"] = humidity;
  
  sensor["name"] = "Sensor 001";
  
  devices["Servo"] = "Servo";
  devices["Led"] = "Led";
  devices["Fan"] = "Fan";

  root["Sensors"] = sensor;
  root["Devices"] = devices;
  
  root.printTo(Serial);
  Serial.println();
  //root.prettyPrintTo(Serial);
  delay(2000);

  String a;
  if (Serial.available() > 0)
  a = Serial.readString();
  if (a == "Fan On") digitalWrite(FAN, HIGH);
  if (a == "Fan Off") digitalWrite(FAN, LOW);
  
  if (a == "Led On") digitalWrite(LED, HIGH);
  if (a == "Led Off") digitalWrite(LED, LOW);

  if (a == "Window Open") servo.write(171);
  if (a == "Window Close") servo.write(0);
}

