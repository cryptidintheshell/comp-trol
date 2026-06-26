#include "new-web.h"
int clients_count = 0;

struct ClientInfo {
	WiFiClient w_client;
	String ip_addr;
	String username;
};


std::vector<ClientInfo> clients_info;	// vec to store wifi clients details
// std::vector<String> clients_names;

String MainPage(std::vector<ClientInfo> &client);

void ConnectToWifi(const char* ssid, const char* passwd) {
	WiFi.begin(ssid, passwd);
	while (WiFi.status() != WL_CONNECTED) {
		delay(200);
	} Serial.println("[+] Connected to the wifi.");
}

void SetupWebpages2(AsyncWebServer &server, std::vector<ClientInfo> &cl) {
	server.on("/", HTTP_GET, [&cl](AsyncWebServerRequest *request) {
		request->send(200, "text/html", MainPage(cl));
	});	

	server.on("/shutdown", HTTP_GET, [&cl](AsyncWebServerRequest *request) {
	    if (request->hasParam("id")) {
	        int id = request->getParam("id")->value().toInt();
	        cl[id].w_client.print("229892");
			request->send(200, "text/plain", "OK");
		}
	});

	server.on("/restart", HTTP_GET, [&cl](AsyncWebServerRequest *request) {
	    if (request->hasParam("id")) {
	        int id = request->getParam("id")->value().toInt();
	        cl[id].w_client.print("893234");
			request->send(200, "text/plain", "OK");	        	
		}
	});	

	server.on("/logoff", HTTP_GET, [&cl](AsyncWebServerRequest *request) {
	    if (request->hasParam("id")) {
	        int id = request->getParam("id")->value().toInt();
	        cl[id].w_client.print("837453");
			request->send(200, "text/plain", "OK");	        	
		}
	});	
}

void HandleConnection(std::vector<ClientInfo> &clients) {
    for (size_t i = 0; i < clients.size(); ) {
        if (!clients[i].w_client.connected()) {
        	Serial.println("[!] Client: " + String(i+1) + " disconnected");
            clients[i].w_client.stop();
            clients.erase(clients.begin() + i);
            continue;
        }

        if (clients[i].w_client.available()) {
            String msg = clients[i].w_client.readStringUntil('\n');
            msg.trim();

            if (msg == "pong") {
                clients[i].w_client.print("ping");
                ++i;
            } else if (msg.startsWith("username:")) {
                String username = msg.substring(9);
                clients[i].username = username;
                Serial.println("[+] New client username: " + username);
                ++i;
            } else {
                Serial.println("[!] Client sent this: " + msg);
                clients[i].w_client.stop();
                clients.erase(clients.begin() + i);
            }
        } else ++i;
    }
}

String MainPage(std::vector<ClientInfo> &client) {
	String html = "<!DOCTYPE html>";
	html += "<html>";
	html += "<head>";
	html += "<meta charset='utf-8'>";
	html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
	html += "<meta http-equiv='refresh' content='3'>";
	html += "<title>Control</title>";
	html += "<style>";
	html += "* { padding: 0; margin: 0; font-family: arial; }";
	html += "body { height: 100vh; display: flex; justify-content: center; align-items: center; }";
	html += ".container { height: 80%; width: 80%; border-radius: 20px; padding: 10px; text-align: center; background: lightgray; }";
	html += ".client-container { margin: 10px; display: grid; grid-template-columns: auto auto; gap: 20px; }";
	html += ".client { padding: 10px; background: limegreen; display: flex; align-items: center; justify-content: space-between; }";
	html += "#disconnected { background: dimgray; }";
	html += ".client-details { text-align: left; }";
	html += ".client-controls { display: flex; justify-content: space-between; }";
	html += ".button { user-select: none; margin-left: 10px; padding: 10px; cursor: pointer; }";
	html += "#shutdown-ctrl { background: red; }";
	html += "#restart-ctrl { background: orange; }";
	html += "#logoff-ctrl { background: lightblue; }";
	html += "#disconnected-ctrl { cursor: not-allowed; background: gray; }";
	html += "</style>";
	html += "</head>";
	html += "<body>";
	html += "<div class='container'>";
	html += "<h1>Command control</h1>";
	html += "<h1>Client count: " + String(client.size()) + "</h1>";

	// Client 1
	if (client.size() == 0) {
		html += "<h1 style='color: dimgray;'>No client connected</h1>";
	} else {
		html += "<div class='client-container'>";
		for (size_t i = 0; i < client.size(); ++i) {
			if (!client[i].w_client.connected()) continue;

			html += "<div class='client' id='client-" + String(i) + "'>";

			html += "<div class='client-details'>";
			html += "<h1>Client " + String(i+1) +"</h1>";
			html += "<h3>PC Name: " + client[i].username + "</h3>";
			html += "<h3>IP: " + client[i].ip_addr + "</h3>";
			html += "</div>";

			html += "<div class='client-controls'>";
			html += "<div onclick='shutdownClient("+String(i)+")' class='button' id='shutdown-ctrl'>Shutdown</div>";
			html += "<div onclick='restartClient("+String(i)+")' class='button' id='restart-ctrl'>Restart</div>";
			html += "<div onclick='logoffClient("+String(i)+")' class='button' id='logoff-ctrl'>Logoff</div>";
			html += "</div>";

			html += "</div>";
			}  
		html += "</div>"; // close client-container
		}  

	html += "</div>"; // close container

	html += "<script>";
    html += "function shutdownClient(id) { fetch('/shutdown?id=' + id); }";
    html += "function restartClient(id) { fetch('/restart?id=' + id); }";
    html += "function logoffClient(id) { fetch('/logoff?id=' + id); }";
    html += "</script>";
    
	html += "</body>";
	html += "</html>";

	return html;
}