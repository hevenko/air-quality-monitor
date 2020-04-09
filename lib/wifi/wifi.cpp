/**
 * wifi.cpp
 * 
 * Wifi connectivity.
 * 
 * created by Tihomir Magdic
 * created 2020-3-15
 * 
 */

#include <wifi.h>
#include <Arduino_JSON.h>
#include <ESP8266WiFi.h>
#include <config.h>
#include <misc.h>

struct sRtcData {
  uint32_t crc32;    // 4 bytes
  uint8_t channel;   // 1 byte,   5 in total
  uint8_t ap_mac[6]; // 6 bytes, 11 in total
  uint8_t padding;   // 1 byte,  12 in total
};

sRtcData rtcData;
bool rtcValid = false;
bool bSoftAP = false;
ESP8266WebServer server(80);
DNSServer dnsServer;

std::map<int,String> encryptionTypes = {  
  { ENC_TYPE_NONE, "Open" },
  { ENC_TYPE_WEP, "WEP" },
  { ENC_TYPE_TKIP, "WPA/PSK" },
  { ENC_TYPE_CCMP, "WPA2/PSK" },
  { ENC_TYPE_AUTO, "WPA/WPA2/PSK" },
};

std::map<int,String> statusTypes = {  
  { WL_CONNECTED, "CONNECTED" }, // connected to a WiFi network
  { WL_NO_SHIELD, "NO SHIELD" }, // no WiFi shield is present
  { WL_IDLE_STATUS, "IDLE STATUS" }, // it is a temporary status assigned when WiFi.begin() is called and remains active until the number of attempts expires (resulting in WL_CONNECT_FAILED) or a connection is established (resulting in WL_CONNECTED);
  { WL_NO_SSID_AVAIL, "NO SSID AVAILABLE" }, // no SSID are available
  { WL_SCAN_COMPLETED, "SCAN COMPLETED" }, // scan networks is completed
  { WL_CONNECT_FAILED, "CONNECT FAILED" }, // connection fails for all the attempts
  { WL_CONNECTION_LOST, "CONNECTION LOST" }, // connection is lost
  { WL_DISCONNECTED, "DISCONNECTED" }, // disconnected from a network
};

void readRTC() {
  // Try to read WiFi settings from RTC memory
  if(ESP.rtcUserMemoryRead(0, (uint32_t*)&rtcData, sizeof(rtcData))) {
    // Calculate the CRC of what we just read from RTC memory, but skip the first 4 bytes as that's the checksum itself.
    uint32_t crc = calculateCRC32(((uint8_t*)&rtcData) + 4, sizeof(rtcData) - 4);
    if(crc == rtcData.crc32) {
      rtcValid = true;
    }
  }
}

bool connectWifi(String ssid, String password) {
  if (bSoftAP)
    return false;
  ssid = ssid == "" ? config.wifissid : ssid;
  if (ssid == "")
    return false;
  password = password == "" ? config.wifipassword : password;
  WiFi.mode(WIFI_STA);
  readRTC();
  if(rtcValid) {
    // The RTC data was good, make a quick connection
    Serial.println("The RTC data was good, make a quick connection");
    WiFi.begin(ssid, password, rtcData.channel, rtcData.ap_mac, true);
  } else {
    // The RTC data was not valid, so make a regular connection
    Serial.println("The RTC data was not valid, so make a regular connection");
    WiFi.begin(ssid, password);
  }

  Serial.printf("Connecting to %s\n", ssid.c_str());

  unsigned long start = millis();
  int timeout = 1000;
  while (WiFi.status() != WL_CONNECTED && --timeout != 0){ // Wait for the Wi-Fi to connect
    delayNonBlocking(10);
    if (timeout % 4)
      Serial.print(".");
    if (timeout == 400) {
      // Quick connect is not working, reset WiFi and try regular connection
      WiFi.disconnect();
      delay(10);
      WiFi.forceSleepBegin();
      delay(10);
      WiFi.forceSleepWake();
      delay(10);
      WiFi.begin(ssid, password);
    }
  }
  Serial.printf("\nConnection time: %lu ms\n", millis() - start);

  Serial.print("\nConnection status:");
  Serial.printf("%d - %s\n", WiFi.status(), translate(statusTypes, WiFi.status()).c_str());
  //Serial.printf("encryptionType: %d\n", WiFi.encryptionType());
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("*** CONNECTION ERROR");
    return false;
  }
  WiFi.persistent(true);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);

  Serial.printf("Connected to %s\n", WiFi.SSID().c_str());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Signal strength: ");
  Serial.println(WiFi.RSSI());
  
  Serial.printf("MAC address: %s\n", WiFi.macAddress().c_str());

  IPAddress subnet = WiFi.subnetMask();
  Serial.print("NetMask: ");
  Serial.println(subnet);

  IPAddress gateway = WiFi.gatewayIP();
  Serial.print("Gateway: ");
  Serial.println(gateway);

  // Write current connection info back to RTC
  rtcData.channel = WiFi.channel();
  memcpy(rtcData.ap_mac, WiFi.BSSID(), 6); // Copy 6 bytes of BSSID (AP's MAC address)
  rtcData.crc32 = calculateCRC32(((uint8_t*)&rtcData) + 4, sizeof( rtcData ) - 4);
  ESP.rtcUserMemoryWrite(0, (uint32_t*)&rtcData, sizeof(rtcData));

  return true;
}

void scanWifiNetworks() {
  int n = WiFi.scanNetworks();
  Serial.printf("%d network(s) found\n", n);
  Serial.printf("  # %-40s Ch\tSignal\tMAC address\tAuth\n", "network");
  Serial.println("-------------------------------------------------------------------------------------------------");
  for (int i = 0; i < n; i++) {
    Serial.printf("%3d %-40s %d\t%ddBm\t%s\t%s\n", i+1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.BSSIDstr(i).c_str(), translate(encryptionTypes, WiFi.encryptionType(i)).c_str());
  }
  Serial.println("-------------------------------------------------------------------------------------------------");
}

String prepare(String s) {
  //Serial.println(s);
  s.replace("{{ap-name}}", config.apssid);
  s.replace("{{apikey}}", config.apikey);
  s.replace("{{wifissid}}", config.wifissid);
  s.replace("{{wifipassword}}", config.wifipassword);
  s.replace("{{device}}", config.devicename + " " + config.deviceversion);
  s.replace("{{firmware-version}}", config.version);
  s.replace("{{config-version}}", config.cfgversion);
  s.replace("{{last-data}}", "last data");
  s.replace("{{last-connection}}", "last connection");
  //Serial.println(s);
  return s;
}

void openSoftAP() {
  if (bSoftAP)
    return;

  WiFi.mode(WIFI_AP_STA);
  Serial.printf("Soft-AP %s %s%s", config.apssid.c_str(), (WiFi.softAP(config.apssid, config.appassword) ? "" : "not "), "established as");
  delay(50);
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  //Serial.printf("MAC address = %s\n", WiFi.softAPmacAddress().c_str());
  server.on("/", [&]() {
    Serial.println("requesting /");
    server.send(200, "text/html", prepare(web_menu).c_str());
  });
  server.on("/style.css", [&]() {
    Serial.println("requesting /style.css");
    server.send(200, "text/css", web_css);
  });
  server.on("/script.js", [&]() {
    Serial.println("requesting /script.js");
    server.send(200, "text/javascript", web_js);
  });
  server.on("/wifi.json", [&]() {
    Serial.println("requesting /wifi.json");

    int n = WiFi.scanNetworks();
    int ix[n]; // array of indices
    for (int i = 0; i < n; i++) 
      ix[i] = i;

    // sort by signal strength
    for (int i = 0; i < n; i++) 
      for (int j = 1; j < n - i; j++) 
        if (WiFi.RSSI(ix[j]) > WiFi.RSSI(ix[j - 1])) 
          std::swap(ix[j], ix[j - 1]);

    for (int i = 0; i < n; i++)
      Serial.printf("%d = %d\n", i, ix[i]);
    //remove duplicates
    //for (int i = 0; i < n; i++) for (int j = i + 1; j < n; j++) if (WiFi.SSID(ix[i]).equals(WiFi.SSID(ix[j])) && WiFi.encryptionType(ix[i]) == WiFi.encryptionType(ix[j])) ix[j] = -1;

    JSONVar web_json;
    for (int i = 0; i < n; i++) {
      JSONVar ssid;
      ssid["ssid"] = WiFi.SSID(ix[i]);
      ssid["strength"] = ((constrain(WiFi.RSSI(ix[i]), -100, -50) + 100) * 2);
      ssid["security"] = translate(encryptionTypes, WiFi.encryptionType(ix[i]));
      ssid["mac"] = WiFi.BSSIDstr(ix[i]);
      ssid["channel"] = WiFi.channel(ix[i]);
      web_json[i] = ssid;
    }

    //server.send(200, "text/plain", web_json);
    //server.send(200, "text/javascript", web_json);
    server.send(200, "application/javascript", JSON.stringify(web_json));
    //server.send(200, "application/json", web_json);
  });
  server.on("/status", [&]() {
    Serial.println("requesting /status");
    server.sendHeader("Cache-Control", " no-cache, no-store, must-revalidate");
    server.sendHeader("Expires", " 0");
    server.send(200, "text/html", prepare(web_status).c_str());
  });
  server.on("/configuration", [&]() {
    Serial.println("requesting /configuration");
    server.sendHeader("Cache-Control", " no-cache, no-store, must-revalidate");
    server.sendHeader("Expires", " 0");
    server.send(200, "text/html", prepare(web_configuration).c_str());
  });
  server.on("/reset", [&]() {
    Serial.println("requesting /reset");
    server.sendHeader("Cache-Control", " no-cache, no-store, must-revalidate");
    server.sendHeader("Expires", " 0");
    server.send(200, "text/html", web_reset);
  });
  server.on("/reset-now", [&]() {
    Serial.println("requesting /reset-now");
    server.sendHeader("Cache-Control", " no-cache, no-store, must-revalidate");
    server.sendHeader("Expires", " 0");
    server.send(200, "text/html", "Resetting...");
    delay(1000);
    ESP.reset();
  });
  server.on("/save", [&]() {
    Serial.println("requesting /save");
    Serial.printf("apikey: %s\n", server.arg("apikey").c_str());
    Serial.printf("ssid: %s\n", server.arg("ssid").c_str());
    Serial.printf("password: %s\n", server.arg("pass").c_str());
    config.apikey = server.arg("apikey");
    config.wifissid = server.arg("ssid");
    config.wifipassword = server.arg("pass");
    writeConfig();
    server.sendHeader("Cache-Control", " no-cache, no-store, must-revalidate");
    server.sendHeader("Expires", " 0");
    server.send(200, "text/html", prepare(web_configuration).c_str());
  });

  server.begin();
  bSoftAP = true;
  delay(100);
  handleSoftAPServer();
}

void handleWebServer() {
	dnsServer.processNextRequest();
	server.handleClient();
}

void handleSoftAPServer() {
  if (bSoftAP) {
    int n = -1;
    int c;
    while(true) {
      yield();
      handleWebServer();
      c = WiFi.softAPgetStationNum();
      if (n != c) {
        n = c;
        Serial.printf("Stations connected to Soft-AP = %d\n", n);
      }
    }
  }
}
