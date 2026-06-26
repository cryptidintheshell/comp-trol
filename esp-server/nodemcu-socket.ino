#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <vector>
#include "web.h"

const char* ssid = "wifi_Wifi";
const char* password = "338171802";
int client_count = 0;

WiFiServer wifi_server(8080);  // Start server on port 8080
AsyncWebServer server(80);
std::vector<WiFiClient> clients;
// WiFiClient client[3];

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", SeeClients(client_count, clients.size()));
  });

  server.on("/shutdown", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("id")) {
        int client_id = request->getParam("id")->value().toInt()-1;
        clients[client_id].println("229892");

        unsigned long timeout = millis() + 1000;
        while (!clients[client_id].available() && millis() < timeout) {
            delay(10);  // Small delay prevents CPU overload
        }

        if (clients[client_id].available()) {
            String recv = clients[client_id].readString();
            Serial.println("Received: " + recv);
            if (recv == "ok") request->send(200, "plain/text", "OK");
        }
    }
  });

  server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("id")) {
        int client_id = request->getParam("id")->value().toInt()-1;
        Serial.println("Client: " + String(client_id));
        clients[client_id].println("893234");
        request->send(200, "plain/text", "OK");
    }    
  });

  wifi_server.begin();
  server.begin();
  Serial.println("Server started...");
}

void loop() {
    WiFiClient new_client = wifi_server.available();

    if (new_client.connected()) {
        // new_client.println("ping");
        clients.push_back(new_client);
        client_count++;
    }

    // listen for ping response
    // for (int i = 0; i < clients.size(); i++) {
    //     unsigned long timeout = millis() + 1000;
    //     while (!clients[i].available() && millis() < timeout) {
    //         delay(10);  // Small delay prevents CPU overload
    //     }

    //     if (clients[i].available()) {
    //         String recv = clients[i].readString();
    //         Serial.println("Received: " + recv);
    //         if (recv == "pong") clients[i].println("ping");
    //     }
    // }

    for (int i = 0; i < clients.size(); i++) {
        if (clients[i] && !clients[i].connected()) {
            clients[i].stop();
            clients.erase(clients.begin() + i);
            Serial.println("Client disconnected.");            
            client_count--;
        } 
    }
}