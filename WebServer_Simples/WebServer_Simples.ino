
#include <DHT_U.h>

// REQUIRES the following Arduino libraries:
// - DHT11 library (DHT11) found at https://github.com/winlinvip/SimpleDHT
// Find All "Great Projects" Videos : https://www.youtube.com/channel/UCCC8DuqicBtP3A_aC53HYDQ/videos


#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "index.h"  //Web page header file
WebServer server(80);
 
// Use the DHT11 temp and humidity sensor
#include <SimpleDHT.h>
int pinDHT11 = 12;
SimpleDHT11 dht11(pinDHT11);

//Enter your SSID and PASSWORD
const char* ssid = "Bruno Galaxy A71";
const char* password = "galaxyA71";
 
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
 
void handleADC() {
                        // send XML file containing input states
                        XML_response();
}
 
void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");
 
/*
//ESP32 As access point
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);
*/
//ESP32 connects to your wifi 
  WiFi.mode(WIFI_STA); //Connectto your wifi
  WiFi.begin(ssid, password);
 
  Serial.println("Connecting to ");
  Serial.print(ssid);
 
  //Wait for WiFi to connect
  while(WiFi.waitForConnectResult() != WL_CONNECTED){      
      Serial.print(".");
    }
    
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  server.on("/", handleRoot);      //This is display page
  server.on("/readADC", handleADC);//To get update of ADC Value only
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
 
void loop()
{   
  server.handleClient();
  delay(1);
}
// send the XML file containing analog value
void XML_response()
{
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    delay(100);
    return;
  }
String content = "<?xml version = \"1.0\" ?>";
    content += "<inputs>";
    content += "<analog>";
    content += (int)temperature;
    content += "</analog>";
    content += "<analog>";
    content += (int)humidity;
    content += "</analog>";
    content += "</inputs>"; 
    server.sendHeader("Cache-Control", "no-cache");
    server.send(200, "text/xml", content); //Send web page
}
