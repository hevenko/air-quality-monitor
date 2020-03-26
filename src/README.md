# Air quality monitor
Arduino based monitor for measuring air quality (PM10, PM2.5, CO, SO2, O3, Pb, NOx, HC, VOC...) and calculating AQI.

To run sketch (.ino) proper configuration is essential.

Sketch reads configuration (from SPISS) and trys to connect to wifi from configuration.
If fails to connect to wifi or wifissid is empty then access point for setup would automatically start up.

### Setup

To access device's setup options, device need to start up access point (after unsuccessfull wifi connection or wifissid from configuration is empty).
In another wifi enabled device (e.g. mobile, laptop, tablet...), scan available wifi networks and find one with name 'Air-Qxxxxxx' (where x's are device's unique hex numbers).
Password to join network is '5A9i0r-Q2u8a7L6i4t3y'. After successfully joining to network, open url at address '192.168.4.1' and setup's main menu should show up.

Setup has three options:
- status - shows current device status
- configuration - check and change apikey and wifi credentials with option for scanning available wifi networks
- reset - resets device - after resetting, device would try to connect with wifi credentials specified in configuration option

### Configuration in the code

- String wifissid - SSID of the network
- String wifipassword - password to connect
- String apssid - soft access point SSID
- String appassword - soft access point password
- long delay - delay value
- String delayUnit - delay unit (possible value is one othe following 'min', 'sec', 'hour')
- String delayType - 'none', 'delay', 'delayNB' (non blocking delay), 'deepsleep'
- uint32_t delayUS - calculated delay as uMS
- String protocol - http or https
- String host - host (e.g. www.domain.com)
- long port - port (e.g. 80)
- String fixedUrl - fixed part url after host and port
- String apikey - api key goes in the request's header
- String version - app version for OTA

Delay type can be configured as:
- none - no delay at all,
- delay - use delay(),
- delayNB - use non-blocking delay, and
- deepsleep - use deepsleep() to minimize power consumption.

For using deep sleep, GPIO 16 should be connected to GND (ESP8266).

For example, to use deep sleep delay type change configuration.cpp or hard-code in initConfig() as:

<code>
config.delayType = "deepsleep";
</code>

### Credentials

Credentials can bi defined in credentials.cpp or hard-coded in .ino in function setupConfig():

Find following line and instead of:

```
  if (!connectWifi(config.wifissid, config.wifipassword))
```

Change it into:

```
  if (!connectWifi("MYSSID", "MYPASS"))
```

### Sensor configuration

To add new sensor, define new sSensor variable at the top of .ino:

```
  sSensor dhtSensor = { NULL, "", 0, 0, 0, 0, 0 };
```

Add new attribute(s) in station variable:

```
struct sStation {
  float temperature; // NEW ATTRIBUTE
  float humidity;    // NEW ATTRIBUTE
  float temperature2;
  float pressure;
  float altitude;
} station;
```
In setupConfig() putcode for sensor instantiation:
```
  if(dhtSensor.handle == NULL) { // check if sensor is not already created (in case of wake from deep sleep)
    // handler, name, port, sensor type, count, minimum delay between sensor reading, internal use
    dhtSensor = { NULL, "DHT11", 2, DHT11, 11, 1000, 0 };
    dhtSensor.handle = new DHT(dhtSensor.pin, dhtSensor.type, dhtSensor.tag); // create sensor
    ((DHT*)dhtSensor.handle)->begin(); // activate sensor
  }
```

Define function for sensor reading:

```
void readTHSensor() {
  waitSensorReading(dhtSensor);
  station.temperature = ((DHT*)dhtSensor.handle)->readTemperature();
  waitSensorReading(dhtSensor);
  station.humidity = ((DHT*)dhtSensor.handle)->readHumidity();
  Serial.printf("\n%s\n", dhtSensor.name.c_str());
  Serial.printf("temperature: %.0f\n", station.temperature);
  Serial.printf("humidity: %.0f\n\n", station.humidity);
}
```

And call that created function in already defined readSensors() function:

```
void readSensors() {
  readTHSensor(); // NEW SENSOR
  readBMPSensor();
}
```

Finally, send sensor's data with http request:

```
  char buffer[50];
  sprintf(buffer, "{\"temperature\":%.0f,\"humidity\":%.0f,\"pressure\":%.0f,\"time\":1}", station.temperature, station.humidity, station.pressure);
  httpPOST("/posts", "", "", String(buffer));
```
