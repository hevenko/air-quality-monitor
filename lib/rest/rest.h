/**
   rest.h
 
   Client REST API and http request implementation with JSON support.
  
   Licensed under MIT license
   Version: 1.0.1

   @file       Rest.h
   @author     Tihomir Magdic
   @license    This project is released under the MIT License (MIT)
   @copyright  Copyright (c) 2020 Tihomir Magdic
   @date       Mar 2020
   @brief

   Version Modified By  Date       Comments
   ------- -----------  ---------- -----------
    1.0.0  T Magdic     15/03/2020 Initial version
    1.0.1  T Magdic     20/10/2020 Added support for JSON (Arduino_JSON library)
*/

#ifndef REST_H
#define REST_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>

// global WiFi client
extern WiFiClient client;

// global HTTP client
extern HTTPClient http;

class HttpRequest {
  public:
    String response; // last raw string response
    int httpCode; // last http code from response
    JSONVar responseJSON; // last raw string response parseed into JSON
    HttpRequest(String); // constructor with resource
    JSONVar request(String, String, String, JSONVar); // general http response call
  protected:
    String resource; // resource
};

/**
 * http request:
 * method - GET, POST, PUT, PATCH, DELETE
 * resource - custom (e.g. "posts")
 * pathParams - custom (e.g. "/posts/1")
 * queryParams - custom (e.g. "fields=a,b,c&order=-b")
 * body - custom (e.g. "{\"temperature\":24,\"humidity:45%\"}") - to convert JSONVar into String use JSON.stringify
 */
String httpRequest(String method, String resource, String pathParams = "", String queryParams = "", String body = "");


/**
 * Following functions are just wrappers around httpRequest() function
 * 
 * http GET request
 */
String httpGET(String resource, String pathParams = "", String queryParams = "");

/**
 * http POST request
 */
String httpPOST(String resource, String pathParams = "", String queryParams = "", String body = "");

/**
 * http PUT request
 */
String httpPUT(String resource, String pathParams = "", String queryParams = "", String body = "");

/**
 * http PATCH request
 */
String httpPATCH(String resource, String pathParams = "", String queryParams = "", String body = "");

/**
 * http DELETE request
 */
String httpDELETE(String resource, String pathParams = "", String queryParams = "", String body = "");

#endif
