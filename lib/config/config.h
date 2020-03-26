/**
 * config.h
 * 
 * Configuration and configuration's functions, sensors and sensor's functions.
 * 
 * created by Tihomir Magdic
 * created 2020-3-15
 * 
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <credentials.h>

struct sConfig {
  String wifissid; // SSID of network's wifi
  String wifipassword; // password of networks's wifi
  String apssid; // soft access point SSID
  String appassword; // soft access point password
  long delay; // delay value
  String delayUnit; // delay unit ('min', 'sec', 'hour')
  String delayType; // 'none', 'delay', 'delayNB' (non blocking delay), 'deepsleep'
  uint32_t delayUS; // calculated delay as uMS
  String protocol; // http or https
  String host; // www.domain.com
  long port; // 80
  String fixedUrl; // fixedurl after host
  String apikey; // api key goes in the header
  String devicename; // device name
  String deviceversion; // device version
  String version; // app version for OTA
  String cfgversion; // configuration version
};

// global config var
extern struct sConfig config;

struct sSensor {
	void* handle; // pointer to sensor object
  String name; // sensor's name (for display usage)
  byte pin; // sensor's pin (if needed for instantiation)
  uint32_t type; // sensor's type (if needed for instantiation)
  uint32_t tag; // additional uint32_t sensor's data
  uint32_t minDelay; // minimum delay in milisec before sensor's reading
  uint32_t lastRead; // last timeclock of sensor's reading (for internal use)
};

/**
 * Prints out (Serial monitor) configuration of config var.
 */
void showConfig();

/**
 * Writes configuration into flash memroy from config var.
 */
bool writeConfig(bool format = true);

/**
 * Reads configuration from flash memory into config var.
 */
bool readConfig();

/**
 * Calculates delayUS attribute in config var.
 */
uint32_t calcDelayUS();

/**
 * Shos sensor struct s as input parameter.
 */
void showSensor(struct sSensor s);

/**
 * Waits for minDelay. Call this before sensor reading.
 */
void waitSensorReading(struct sSensor s);

#endif
