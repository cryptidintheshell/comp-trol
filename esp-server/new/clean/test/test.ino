#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

WiFiServer wifi_server(4535);
AsyncWebServer web_server(80);

std::vector<WiFiClient> clients;

const char* ssid = "wifi_Wifi";
const char* passwd = "338171802"; 

void setup() {
	Serial.begin(115200);

	Serial.print("[~] Connecting to the network");
	WiFi.begin(ssid, passwd);
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
	} Serial.println("\n[+] Connected to the network.");

	web_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "text/plain", "Hellow");
	});

	wifi_server.begin();
	web_server.begin();
}

void loop() {
	WiFiClient new_client = wifi_server.available();

	if (new_client.connected()) {
		clients.push_back(new_client);
	}

	for (int i = 0; i < clients.size(); i++) {
		if (clients[i].connected()) {
			clients[i].print("ping");
		} else if (clients[i].connected() && clients[i].available()) {
			String msg = clients[i].readString();
			if (msg == "pong") clients[i].print("ping");
		}
	}

}