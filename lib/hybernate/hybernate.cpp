/**
 * hybernate.cpp
 * 
 * Hybernate functions.
 * 
 * created by Tihomir Magdic
 * created 2020-3-15
 * 
 */

#include <hybernate.h>
#include <ESP8266WiFi.h>
#include <config.h>
#include <misc.h>

void sleep() {
  Serial.println("sleeping...");
	
	WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  yield();

	/*
	  WiFi.disconnect();
    WiFi.enableAP(false);
    WiFi.enableSTA(false);
    WiFi.forceSleepBegin();
    delay(1);
	*/
	
  WiFi.disconnect(true);  // if this line is missing, you wont get to the promised 20 uA current consumption while sleeping
  yield();
  // 'none', 'delay', 'delayNB' (non blocking delay), 'deepsleep'
	Serial.printf("sleep time: %ud US (%ld %s)\n", config.delayUS, config.delay, config.delayUnit.c_str());
	if(config.delayType == "delay") {
		delay(config.delayUS / 1000);
	} else if(config.delayType == "delayNB") {
		delayNonBlocking(config.delayUS / 1000);
	} else if(config.delayType == "deepsleep") {
	  ESP.deepSleep(config.delayUS);
	}

  yield();
}

void wake() {
  WiFi.forceSleepWake(); // Wake up wifi-chip from sleep
}
