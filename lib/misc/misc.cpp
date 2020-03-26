/**
 * misc.cpp
 * 
 * Misc functions.
 * 
 * created by Tihomir Magdic
 * created 2020-3-15
 * 
 */

#include <misc.h>

uint32_t calculateCRC32(const uint8_t *data, size_t length) {
  uint32_t crc = 0xffffffff;
  while(length--) {
    uint8_t c = *data++;
    for(uint32_t i = 0x80; i > 0; i >>= 1) {
      bool bit = crc & 0x80000000;
      if(c & i) {
        bit = !bit;
      }

      crc <<= 1;
      if(bit) {
        crc ^= 0x04c11db7;
      }
    }
  }

  return crc;
}

void delayNonBlocking(unsigned long pause, GeneralFunction cb) {
  unsigned long previousMillis = millis();
  while ((millis() - previousMillis) < pause) {
    yield();
    if (cb != NULL)
      cb();
    delay(10);
  }
}

String translate(std::map<int,String> m, int value) {
  std::map<int, String>::iterator found = m.find(value);
  if (found == m.end())
    return "";
  else
    return found->second;
}

void showESPInfo() {
  Serial.println("ESP info:");
  Serial.print("ESP.getChipId(): ");
  Serial.println(ESP.getChipId());
  Serial.print("ESP.getCoreVersion(): ");
  Serial.println(ESP.getCoreVersion());
  Serial.print("ESP.getSdkVersion(): ");
  Serial.println(ESP.getSdkVersion());
  Serial.print("ESP.getCpuFreqMHz(): ");
  Serial.println(ESP.getCpuFreqMHz());
  Serial.print("ESP.getSketchSize(): ");
  Serial.println(ESP.getSketchSize());
  Serial.print("ESP.getFreeSketchSpace(): ");
  Serial.println(ESP.getFreeSketchSpace());
  Serial.print("ESP.getFlashChipId(): ");
  Serial.println(ESP.getFlashChipId());
  Serial.print("ESP.getFlashChipSize(): ");
  Serial.println(ESP.getFlashChipSize());
  Serial.print("ESP.getFlashChipRealSize(): ");
  Serial.println(ESP.getFlashChipRealSize());
  Serial.print("ESP.getFlashChipSpeed(void): ");
  Serial.println(ESP.getFlashChipSpeed());
  Serial.print("ESP.getCycleCount(): ");
  Serial.println(ESP.getCycleCount());
  Serial.print("ESP.getVcc(): ");
  Serial.println(ESP.getVcc());
  Serial.printf("ESP.deepSleepMax(): %lld\n", ESP.deepSleepMax());
}

String ensureLeft(String s, String leading) {
  return s.startsWith(leading) ? s : leading + s;
}

String ensureRight(String s, String trailing) {
  return s.endsWith(trailing) ? s : s + trailing;
}

String ensureBoth(String s, String both) {
  return ensureRight(ensureLeft(s, both), both);
}

String ensureLeftTrimRight(String s, String both) {
  return ensureLeftTrimRight(s, both, both);
}

String ensureLeftTrimRight(String s, String leading, String trailing) {
  return trimRight(ensureLeft(s, leading), trailing);
}

String trimLeftEnsureRight(String s, String both) {
  return trimLeftEnsureRight(s, both, both);
}

String trimLeftEnsureRight(String s, String leading, String trailing) {
  return trimLeft(ensureRight(s, trailing), leading);
}

String trimLeft(String s, String leading) {
	return s.startsWith(leading) ? trimLeft(s.substring(leading.length()), leading) : s;
}

String trimRight(String s, String trailing) {
	return s.endsWith(trailing) ? trimRight(s.substring(0, s.length() - trailing.length()), trailing) : s;
}

String trimBoth(String s, String both) {
  return trimBoth(s, both, both);
}

String trimBoth(String s, String leading, String trailing) {
  return trimRight(trimLeft(s, leading), trailing);
}

String formatAsFloat(double value, byte decimals) {
  char buffer[50];
  String format = "%." + String(decimals) + "f";
  sprintf(buffer, format.c_str(), value);
  return String(buffer);
}
