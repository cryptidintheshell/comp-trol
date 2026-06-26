#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "new.h"

const char* ssid = "wifi_Wifi";
const char* passwd = "338171802"; 
int client_count = 0;

WiFiServer wifi_server(8080);
AsyncWebServer web_server(80);
std::vector<WiFiClient> clients;
String message = "";

void setup() {
	Serial.begin(115200);
	Serial.println("Begin");

	WiFi.hostname("nodee");

	ConnectToWifi(ssid, passwd);
	SetupWebpages(web_server, clients);

	web_server.begin();
	wifi_server.begin();

	Serial.println("[+] Everythings is good, handling connections now.");
}

void loop() {
	WiFiClient new_client = wifi_server.available();

	if (new_client.connected()) {
		if (new_client.connected() && new_client.available()) {
			String msg = new_client.readString();
			// if(msg == "") // if its hostname, store it and push it to another vector of hostnames 

			String client_ip = new_client.remoteIP().toString(); 
			Serial.println("[+] A client is connected.");
			Serial.println("[+] Client ip address: " + client_ip);
			
			// new_client.print(client_ip);
			clients.push_back(new_client);
			new_client.print("ping");
		}
	}

	HandleConnection(clients, message);
}