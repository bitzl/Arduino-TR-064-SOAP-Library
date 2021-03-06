/**
 * caller.ino
 *  Oliver-André Urban
 * based on
 * home-indicator.ino
 *  by René Vollmer
 *  Example code for the home-indicator-project
 *  
 * many thanks to René for his TR-064 library 
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <tr064.h>

#define USE_SERIAL Serial
#define BUTTON D3 
// Flash BUTTON - you can connect a seperate button to D3 or an opto-coupler 
// for example: use a resistor and an opto-coupler to connect to a doorbell

ESP8266WiFiMulti WiFiMulti;


//-------------------------------------------------------------------------------------
//Put your router settings here
//-------------------------------------------------------------------------------------


//Wifi network name (SSID)
const char* wifi_ssid = "WLANSID"; 

//Wifi network password
const char* wifi_password = "XXXXXXXXXXXXXXXXXXXXX";

//The username and password for the API of your router
//The username if you created and account, "admin" otherwise
//(Anmeldung aus dem Heimnetz mit Benutzername und Passwort)
const char* fuser = "homechecker";
const char* fpass = "this_shouldBEaDecentPassword!";

//IP address of your router. This should be "192.168.178.1" for most FRITZ!Boxes
const char* IP = "192.168.178.1";

//Port of the API of your router. This should be 49000 for all TR-064 devices.
const int PORT = 49000;


//-------------------------------------------------------------------------------------

//Do not mess with these :)
//TR-064 connection
TR064 connection(PORT, IP, fuser, fpass);


void setup() {

  //Wait a few secs for warm-up (dunno why, was in the default code for http connections).
  delay(5000);
 
  //Connect to wifi
  WiFiMulti.addAP(wifi_ssid, wifi_password);

  //Wait for the wifi to connect
  while ((WiFiMulti.run() != WL_CONNECTED)) {
    delay(100);
  }
  pinMode(BUTTON, INPUT); // Port as input
}

void loop() {
  int taste = digitalRead(BUTTON);
  if (digitalRead(BUTTON)== LOW) 
  {
    USE_SERIAL.println();
    USE_SERIAL.printf("Button pressed");
    callWahlhilfe();
    delay(20000);
  }
  else
  {
    USE_SERIAL.println();
    USE_SERIAL.printf("Button not pressed");
    delay(50);   
  }
  //delay(100);   
}


int callWahlhilfe() {
  // Maybe the WLAN connection is lost, so testing an reconnect if needed
  if ((WiFiMulti.run() != WL_CONNECTED)) {
    WiFiMulti.addAP(wifi_ssid, wifi_password);
    while ((WiFiMulti.run() != WL_CONNECTED)) {
      delay(100);
    }
  }
  //(Re-) Initialize the TR-064 library - it is done every time, as maybe the connection has lost before
  connection.init();

  String params[][2] = {{"NewX_AVM-DE_PhoneNumber", "**799"}};
  String req[][2] = {{}};

  connection.action("urn:dslforum-org:service:X_VoIP:1", "X_AVM-DE_DialNumber", params, 1, req, 0);
}
