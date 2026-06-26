#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "new.h"

const char* ssid = "Pisonet";
const char* passwd = "338171802"; 

WiFiServer wifi_server(8080);
AsyncWebServer web_server(80);

std::vector<WiFiClient> clients;

void setup() {
	Serial.begin(115200);
	Serial.println("Begin");

	WiFi.hostname("nodee");

	ConnectToWifi(ssid, passwd);
	SetupWebpages2(web_server, clients_info);

	web_server.begin();
	wifi_server.begin();

	Serial.println("[+] Everythings is good, handling connections now.");
}

void loop() {
	WiFiClient new_client = wifi_server.available();

	if (new_client && new_client.connected()) {
		ClientInfo c_info;

		c_info.w_client = new_client;					 		// store WiFiClient object
		c_info.ip_addr = new_client.remoteIP().toString();		// store ip addr

		clients_info.push_back(c_info);
	}

	HandleConnection(clients_info);
}
