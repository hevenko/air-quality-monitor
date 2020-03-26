/**
 * wifi.h
 * 
 * Wifi connectivity.
 * 
 * created by Tihomir Magdic
 * created 2020-3-15
 */

#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <map>
#include <hybernate.h>
#include <web.h>

// global is access point is active
extern bool bSoftAP;

// global server for access point - serves client requests
extern ESP8266WebServer server;

// global dns server
extern DNSServer dnsServer;

// global encryption types mapping
extern std::map<int,String> encryptionTypes;

// global dns port
const byte DNS_PORT = 53;

/**
 * connect to WiFi with SSID and password
 */
bool connectWifi(String ssid = "", String password = "");

/**
 * Scans WiFi networks to allow user to choose network SSID
 */
void scanWifiNetworks();

/**
 * Opens access point with credentials in config.apssid and config.appassword
 */
void openSoftAP();

/**
 * Handles user requests in access point mode.
 */
void handleSoftAPServer();

#endif
