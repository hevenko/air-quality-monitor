/**
 * credentials.h
 * 
 * Credentials for wifi and access point.
 * 
 * created by Tihomir Magdic
 * created 2020-3-15
 * 
 */

#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <Arduino.h>

struct sCredentials {
	String wifissid; // wifi SSID
	String wifipassword; // wifi password
  String apssid; // SSID of access point
	String appassword; // password for access point
	//String apikey; // device's api key
};

// global credentials
extern sCredentials credentials;

#endif
