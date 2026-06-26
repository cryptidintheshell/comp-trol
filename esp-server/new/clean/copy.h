// void SetupWebpages(AsyncWebServer &server, std::vector<WiFiClient> &clients) {
// 	server.on("/", HTTP_GET, [&clients, clients_names](AsyncWebServerRequest *request) {
// 		Serial.println("[!] Client name: " + clients_names[0]);
// 		request->send(200, "text/html", MainPage(clients));
// 	});

// 	server.on("/shutdown", HTTP_GET, [&clients](AsyncWebServerRequest *request) {
// 	    if (request->hasParam("id")) {
// 	        int client_id = request->getParam("id")->value().toInt();
// 	        clients[client_id].print("229892");
// 			request->send(200, "text/plain", "OK");
// 		}
// 	});

// 	server.on("/restart", HTTP_GET, [&clients](AsyncWebServerRequest *request) {
// 	    if (request->hasParam("id")) {
// 	        int client_id = request->getParam("id")->value().toInt();
// 	        clients[client_id].print("893234");
// 			request->send(200, "text/plain", "OK");	        	
// 		}
// 	});	

// 	server.on("/logoff", HTTP_GET, [&clients](AsyncWebServerRequest *request) {
// 	    if (request->hasParam("id")) {
// 	        int client_id = request->getParam("id")->value().toInt();
// 	        clients[client_id].print("837453");
// 			request->send(200, "text/plain", "OK");	        	
// 		}
// 	});	
// }

// void HandleConnection(std::vector<WiFiClient> &clients) {
//     for (int i = 0; i < clients.size(); ) {
//         if (!clients[i].connected()) {
//         	Serial.println("[!] Client: " + String(i+1) + "disconnected");
//             clients[i].stop();
//             clients.erase(clients.begin() + i);
//             continue;
//         }

//         if (clients[i].available()) {
//             String msg = clients[i].readStringUntil('\n');
//             msg.trim();

//             if (msg == "pong") {
//                 clients[i].print("ping");
//                 ++i;
//             } else if (msg.startsWith("username:")) {
//                 String username = msg.substring(9);
//                 clients_names.push_back(username);
//                 Serial.println("[+] New client username: " + username);
//                 ++i;
//             } else {
//                 Serial.println("[!] Client sent this: " + msg);
//                 clients[i].stop();
//                 clients.erase(clients.begin() + i);
//                 clients_names.erase(clients_names.begin() + i);
//             }
//         } else ++i;
//     }
// }