/*
 * Air quality measuring
 * prototype sketch for communicating with API
 * 
 * wake -> read sensors data -> send data -> sleep -> wake
 * 
 * version 1.0
 * created by Tihomir Magdic
 * created 2020-3-15
 * 
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <Arduino_JSON.h>
#include <Adafruit_BMP085.h>
#include <rest.h>
#include <hybernate.h>
#include <wifi.h>
#include <misc.h>
#include <config.h>

struct sSensor dhtSensor = { NULL, "", 0, 0, 0, 0, 0 };
sSensor bmpSensor = { NULL, "", 0, 0, 0, 0, 0 };

uint32_t counter = 0;

struct sStation {
  float temperature;
  float humidity;
  float temperature2;
  float pressure;
  float altitude;
} station;

void readTHSensor() {
  waitSensorReading(dhtSensor);
  station.temperature = ((DHT*)dhtSensor.handle)->readTemperature();
  waitSensorReading(dhtSensor);
  station.humidity = ((DHT*)dhtSensor.handle)->readHumidity();
  Serial.printf("\n%s\n", dhtSensor.name.c_str());
  Serial.printf("temperature: %.0f\n", station.temperature);
  Serial.printf("humidity: %.0f\n", station.humidity);
}

void readBMPSensor() {
  waitSensorReading(bmpSensor);
  station.temperature2 = ((Adafruit_BMP085*)bmpSensor.handle)->readTemperature();
  waitSensorReading(bmpSensor);
  station.pressure = ((Adafruit_BMP085*)bmpSensor.handle)->readPressure();
  waitSensorReading(bmpSensor);
  station.altitude = ((Adafruit_BMP085*)bmpSensor.handle)->readAltitude();
  Serial.printf("\n%s\n", bmpSensor.name.c_str());
  Serial.printf("temperature2: %.0f\n", station.temperature2);
  Serial.printf("pressure: %.0f\n", station.pressure);
  Serial.printf("altitude: %.0f\n\n", station.altitude);
}

void readSensors() {
  readTHSensor();
  readBMPSensor();
}

void initConfig() {
  //showESPInfo();
  //showConfig();
  //writeConfig();
  readConfig();
  
  showConfig();
  //Serial.println();

  //scanWifiNetworks();
  //Serial.println("waiting for 10 seconds");
  //delayNonBlocking(10000);
  //Serial.println("waiting another 2 seconds");
  //delayNonBlocking(2000);
}

void setupConfig() {
  Serial.begin(115200);
  delayNonBlocking(10);
  while (!Serial) yield();
  Serial.printf("\nESP_getChipId(): %d\n", ESP.getChipId());
  
  Serial.println("setup");
  Serial.printf("\nBoard: %s\n\n", ARDUINO_BOARD);
  String resetReason = ESP.getResetReason();
  Serial.printf("Reset reason: %s\n", resetReason.c_str());

  if(dhtSensor.handle == NULL) { // check if sensor is not already created (in case of wake from deep sleep)
    // handler, name, port, sensor type, count, minimum delay between sensor reading, internal use
    dhtSensor = { NULL, "DHT11", 2, DHT11, 11, 1000, 0 };
    dhtSensor.handle = new DHT(dhtSensor.pin, dhtSensor.type, dhtSensor.tag); // create sensor
    ((DHT*)dhtSensor.handle)->begin(); // activate sensor
    showSensor(dhtSensor);
    Serial.println();
  }
  
  if(bmpSensor.handle == NULL) {
    bmpSensor = { new Adafruit_BMP085(), "BMP180", 0, 0, 0, 1000, 0 };
    ((Adafruit_BMP085*)bmpSensor.handle)->begin();
    showSensor(bmpSensor);
    Serial.println();
  }

  if (resetReason != "Deep-Sleep Wake") {
    wake();
    initConfig();
  }

  Serial.printf("WiFi.SSID(): %s\n", WiFi.SSID().c_str());
  Serial.printf("ESP_getChipId(): %d\n", ESP.getChipId());
  Serial.printf("ESP_getChipId(): %s\n", String(ESP.getChipId(), HEX).c_str());

  Serial.printf("SSID: %s\n", WiFi.SSID().c_str());
  Serial.printf("Password: %s\n", WiFi.psk().c_str());
  Serial.print("Subnet mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.printf("Gataway IP: %s\n", WiFi.gatewayIP().toString().c_str());
  Serial.print("WiFi.dnsIP(0): ");
  WiFi.dnsIP(0).printTo(Serial);
  Serial.print("WiFi.dnsIP(1): ");
  WiFi.dnsIP(1).printTo(Serial);

  if (!connectWifi())
    //WiFi.softAPdisconnect(true);
    openSoftAP();
}

void testJSON() {
  String json1 = "{\"temperature\":\"foo\",\"humidity\":\"bar\",\"time\":1,\"temperature2\":120}";
  Serial.printf("json1: %s\n", json1.c_str());
  
  JSONVar myObject = JSON.parse(json1);
  if (JSON.typeof(myObject) == "undefined") {
    Serial.println("Parsing input failed!");
    return;
  }
  
  Serial.print("JSON.typeof(myObject) = ");
  Serial.println(JSON.typeof(myObject)); // prints: object
  
  Serial.print("JSON.typeof(myObject[temperautre2]) = ");
  Serial.println(JSON.typeof(myObject["temperature2"]));
  
  Serial.print("myObject = ");
  Serial.println(myObject);

  if (myObject.hasOwnProperty("temperature")) {
    Serial.print("myObject[\"temperature\"] = ");
    Serial.println((const char*) myObject["temperature"]);
  }
  
  if (myObject.hasOwnProperty("temperature2")) {
    Serial.print("myObject[\"temperature2\"] = ");
    Serial.println((int) myObject["temperature2"]);
  }

  JSONVar myObject2;

  myObject2["hello"] = "world";
  myObject2["true"] = true;
  myObject2["x"] = 42;
  myObject2["temperature"] = station.temperature;
  myObject2["temperature-0"] = formatAsFloat(station.temperature, 0).toDouble();
  myObject2["temperature-1"] = formatAsFloat(station.temperature, 1).toDouble();
  myObject2["temperature-2"] = formatAsFloat(station.temperature, 2);
  myObject2["temperature-9"] = formatAsFloat(station.temperature, 9).toDouble();
  myObject2["humidity"] = station.humidity;
  myObject2["temperature2"] = station.temperature2;
  myObject2["pressure"] = station.pressure;
  myObject2["altitude"] = station.altitude;

  Serial.print("myObject.keys() = ");
  Serial.println(myObject2.keys());

  // JSON.stringify(myVar) can be used to convert the json var to a String
  String jsonString = JSON.stringify(myObject2);

  Serial.print("JSON.stringify(myObject2) = ");
  Serial.println(jsonString);
}

void setup() {
  setupConfig();
}

void loop() {
  //Serial.print("loop");
  //Serial.println(++counter);
  //delayNonBlocking(2000, handleWebServer);
  //handleSoftAPServer();
  //delay(100);
  //handleSoftAPServer();
  //readSensors();

  //testJSON();

  //httpGET("/typicode/demo/posts/1");
  //httpDELETE("/typicode/demo/posts/1");
  //httpGET("posts", "1");
  //httpDELETE("posts", "1", "fields=abc");
  //httpPOST("/posts", "", "", "{\"temperature\":\"foo\",\"humidity\":\"bar\",\"time\":1}");
  //httpPATCH("/posts", "1", "", "{\"title\":\"foo\",\"body\":\"bar\",\"userId\":1}");
  /*
  char buffer[200];
  sprintf(buffer, "{\"temperature\":%.0f,\"humidity\":%.0f,\"pressure\":%.0f,\"time\":1}", station.temperature, station.humidity, station.pressure);

  Serial.println("object post request");
  JSONVar JSONData;
  JSONData["temperature"] = formatAsFloat(station.temperature, 0).toDouble();
  JSONData["humidity"] = station.humidity;
  JSONData["pressure"] = station.pressure;
  JSONData["time"] = 1;
  JSONVar httpResponse = post.send(JSONData);
  Serial.printf("httpResponse httpcode: %d\n", post.httpCode);
  Serial.printf("httpResponse httpcode name: %s\n", http.errorToString(post.httpCode).c_str());
  Serial.printf("httpResponse stringified: %s\n", JSON.stringify(httpResponse).c_str());

  Serial.println("regular post request");
  String data = String(buffer);
  httpPOST("/posts", "", "", data);
  */
  //Serial.printf("WiFi.getTime(): %d\n", WiFi.getTime());
  // Serial.printf("Current WiFi Firmware: %s", WiFi.firmwareVersion());

  //sleep();
}
