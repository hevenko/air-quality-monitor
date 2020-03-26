/**
 * rest.cpp
 * 
 * Client REST API implementation
 * 
 * created by Tihomir Magdic
 * created 2020-3-15
 * 
 */

#include <stdint.h>
#include <rest.h>
#include <config.h>
#include <misc.h>

WiFiClient client;
HTTPClient http;
int lastHttpCode;


HttpRequest::HttpRequest(String r) {
  resource = ensureLeftTrimRight(r, "/");
}

JSONVar HttpRequest::request(String method, String pathParams, String queryParams, JSONVar body) {
  response = httpRequest(method, resource, pathParams, queryParams, JSON.stringify(body));
  responseJSON = JSON.parse(response);
  httpCode = lastHttpCode;
  return responseJSON;
}

String httpRequest(String method, String resource, String pathParams, String queryParams, String body) {
  //HTTPClient http;
  // my-json-server.typicode.com/typicode/demo/posts?fields=id
  String host = config.host; // "my-json-server.typicode.com";
  uint16_t port = config.port; // 80;
  Serial.printf("http request: %s\n", method.c_str());
	/*
  Serial.printf("host: %s\n", host.c_str());
  Serial.printf("port: %s\n", String(port).c_str());
  Serial.printf("resource: %s\n", String(resource).c_str());
  Serial.printf("pathParams: %s\n", String(pathParams).c_str());
  Serial.printf("queryParams: %s\n", String(queryParams).c_str());
	*/
  String url = config.fixedUrl + ensureLeftTrimRight(resource, "/") + ensureLeftTrimRight(pathParams, "/") + ((queryParams.length() > 0) ? ensureLeft(queryParams, "?") : "");
  //Serial.printf("url: %s\n", url.c_str());
  if (body.length())
    Serial.printf("http body: %s\n", body.c_str());
  http.begin(client, host, port, url);
  http.addHeader("Content-type", "application/json; charset=UTF-8");
  http.addHeader("X-APIKEY", config.apikey);

  lastHttpCode = http.sendRequest(method.c_str(), body);
  Serial.printf("[HTTP] code: %d %s\n", lastHttpCode, http.errorToString(lastHttpCode).c_str());
  String payload;
  if (lastHttpCode > 0) {
		payload = http.getString();
		Serial.println(payload);
  } else {
    Serial.printf("[HTTP] failed, error: %s\n", http.errorToString(lastHttpCode).c_str());
  }  
  Serial.println("closing connection");
  http.end();
  //WiFi.disconnect();
  return payload;
}

String httpGET(String resource, String pathParams, String queryParams) {
  return httpRequest("GET", resource, pathParams, queryParams);
}

String httpPOST(String resource, String pathParams, String queryParams, String body) {
  return httpRequest("POST", resource, pathParams, queryParams, body);
}

String httpPUT(String resource, String pathParams, String queryParams, String body) {
  return httpRequest("PUT", resource, pathParams, queryParams, body);
}

String httpPATCH(String resource, String pathParams, String queryParams, String body) {
  return httpRequest("PATCH", resource, pathParams, queryParams, body);
}

String httpDELETE(String resource, String pathParams, String queryParams, String body) {
  return httpRequest("DELETE", resource, pathParams, queryParams, body);
}


String httpRequest0(String method, String url, String body) {
  String host = "my-json-server.typicode.com";
  if (client.connect(host, 80))
  {  
    String apiWritekey = "*************"; 
    String Data = apiWritekey;
           Data +="&field1=";
           Data += String(101);
           Data += "\r\n\r\n";
 
     client.print(method + " " + url + " HTTP/1.1\n");
     client.print("Host: " + host + "\n");
     client.print("Connection: close\n");
     //client.print("X-THINGSPEAKAPIKEY: "+apiWritekey+"\n");
     client.print("Content-Type: application/json; charset=UTF-8\n");
     client.print("Content-Length: ");
     client.print(body.length());
     client.print("\n\n");
     client.print(body);
     client.print("\n\n");
     Serial.println("sending to " + host);
  }
  delay(500);
  
  // Read all the lines of the reply from server and print them to Serial
  Serial.println("waiting for respond");
  String line = "";
  while(client.available()){
    //line += client.readStringUntil('\r');
    line += client.readString();
  }
  Serial.println("response: ");
  Serial.println(line);
  Serial.println("content: ");
  int p1 = line.indexOf("\r\n\r\n");
  //Serial.println(p1);
  String result = line.substring(p1 + 4);
  Serial.println(result);
  client.stop();
  return result;
}
