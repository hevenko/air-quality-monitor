/**
 * config.cpp
 * 
 * Configuration and configuration's functions, sensors and sensor's functions.
 * 
 * created by Tihomir Magdic
 * created 2020-3-15
 * 
 */

#include <Arduino.h>
#include <FS.h>
#include <config.h>
#include <misc.h>

struct sConfig config = {
  credentials.wifissid, credentials.wifipassword,
  "Air-Q" + String(ESP.getChipId(), HEX), "5A9i0r-Q2u8a7L6i4t3y",
  // credentials.apssid, credentials.appassword,
  5, "sec", "delayNB", calcDelayUS(), // 'none', 'delay', 'delayNB' (non blocking delay), 'deepsleep'
  //10, "sec", "deepsleep", calcDelayUS(), // 'none', 'delay', 'delayNB' (non blocking delay), 'deepsleep'
  "http", trimRight("my-json-server.typicode.com", "/"), 80, ensureLeftTrimRight("/typicode/demo/", "/"),
  "xefh-34lp-rw91-cb32",
  "Airduino", "1.0", "2.0", "3.0"
};

//struct sConfig config = {};

void showConfig() {
  Serial.println("***** Configuration");
  Serial.printf("wifissid: %s\n", config.wifissid.c_str());
  Serial.printf("wifipassword: %s\n", config.wifipassword.c_str());
  Serial.printf("apssid: %s\n", config.apssid.c_str());
  Serial.printf("appassword: %s\n", config.appassword.c_str());
  Serial.printf("delay: %ld\n", config.delay);
  Serial.printf("delayUnit: %s\n", config.delayUnit.c_str());
  Serial.printf("delayType: %s\n", config.delayType.c_str());
  Serial.printf("protocol: %s\n", config.protocol.c_str());
  Serial.printf("host: %s\n", config.host.c_str());
  Serial.printf("port: %ld\n", config.port);
  Serial.printf("fixedUrl: %s\n", config.fixedUrl.c_str());
  Serial.printf("apikey: %s\n", config.apikey.c_str());
  Serial.printf("version: %s\n", config.version.c_str());
}

bool writeConfig(boolean format) {
  Serial.println("writing config...");
  SPIFFS.begin();
  File f = SPIFFS.open(String("/config").c_str(), "w");
  if (!f) {
    Serial.println("config file open for writing failed");
    if (format) {
      Serial.println("formatting...");
      SPIFFS.format();
      Serial.println("formatted");
      return writeConfig(false);
    } else
    	return false;
  }
	f.print(config.wifissid + '\0');
	f.print(config.wifipassword + '\0');
	f.print(config.apssid + '\0');
	f.print(config.appassword + '\0');
	f.print(String(config.delay) + '\0');
	f.print(config.delayUnit + '\0');
	f.print(config.delayType + '\0');
	f.print(config.protocol + '\0');
	f.print(config.host + '\0');
	f.print(String(config.port) + '\0');
	f.print(config.fixedUrl + '\0');
	f.print(config.apikey + '\0');
	f.print(config.version + '\0');
  f.close();
  SPIFFS.end();
	return true;
}

uint32_t calcDelayUS() {
 	if (config.delayUnit == "sec") {
    return config.delay * 1000000;
  } else if (config.delayUnit == "min") {
    return config.delay * 60 * 1000000;
  } else if (config.delayUnit == "hour") {
    return config.delay * 60 * 60 * 1000000;
  } else {
    return 0;
  }
}

bool readConfig() {
  bool result = false;
  Serial.println("reading config...");
  SPIFFS.begin();
  File f = SPIFFS.open(String("/config").c_str(), "r");
  if (!f) {
    Serial.println("config file open failed");
  } else {
    Serial.println("reading data from File:");
		config.wifissid = f.readStringUntil('\0');
		config.wifipassword = f.readStringUntil('\0');
		config.apssid = f.readStringUntil('\0');
		config.appassword = f.readStringUntil('\0');
		config.delay = f.readStringUntil('\0').toInt();
		config.delayUnit = f.readStringUntil('\0');
		config.delayType = f.readStringUntil('\0');
		config.protocol = f.readStringUntil('\0');
		config.host = f.readStringUntil('\0');
		config.port = f.readStringUntil('\0').toInt();
		config.fixedUrl = f.readStringUntil('\0');
		config.apikey = f.readStringUntil('\0');
		config.version = f.readStringUntil('\0');

		config.delayUS = calcDelayUS();
  	config.host = trimRight(config.host, "/");
  	config.fixedUrl = ensureLeftTrimRight(config.fixedUrl, "/");

    f.close();
    result = true;
    Serial.println("config file closed");
  }
  SPIFFS.end();
  return result;
}

void showSensor(struct sSensor s) {
  Serial.println("***** Sensor");
  Serial.printf("name: %s\n", s.name.c_str());
  Serial.printf("pin: %d\n", s.pin);
  Serial.printf("type: %d\n", s.type);
  Serial.printf("tag: %d\n", s.tag);
  Serial.printf("minDelay: %d\n", s.minDelay);
  Serial.printf("lastRead: %d\n", s.lastRead);
}

void waitSensorReading(struct sSensor s) {
  uint32_t now = millis();
  if ((now - s.lastRead) < s.minDelay)
    delayNonBlocking(s.minDelay - (now - s.lastRead));
  s.lastRead = millis();
}