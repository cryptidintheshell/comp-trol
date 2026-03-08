#pragma once

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/filedlg.h>
#include <wx/sizer.h>
#include <string.h>
#include <vector>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <mutex>

const auto wxDP = wxDefaultPosition;
const auto wxDS = wxDefaultSize;

class Window : public wxFrame {
	
	wxButton* start_button;
	wxButton* shutdown_button;
	wxButton* restart_button;
	wxButton* lock_button;
	wxButton* close_program_button;
	wxButton* send_file_button;

	wxTextCtrl* update_field;

	wxPanel* main_panel;
	wxPanel* buttons_panel;
	wxPanel* cmd_panel;
	wxPanel* update_panel;
	wxPanel* client_panel;

	wxBoxSizer* main_sizer;
	wxBoxSizer* buttons_panel_sizer;
	wxBoxSizer* update_panel_sizer;
	wxStaticBoxSizer* cmd_panel_sizer;
	wxBoxSizer* client_panel_sizer;
	wxBoxSizer* vertical_sizer;

	wxGrid* clients_grid;

	std::vector<int> client_sockets;
 	std::vector<std::string> client_address;
 	int client_count = 0;
 	int current_socket = 0;
 	std::string current_client = "";

	struct sockaddr_in server_address;
	int server_socket = 0;
	std::mutex client_mutex;

public:
	Window(const wxString &title, wxPoint wPoint, wxSize wSize);

	void HandleIncomingConnection();
	void HandleClient(int socket, std::string ip, int pos);

	void StartServer(wxCommandEvent &event);
 	void OnButtonShutdown(wxCommandEvent &event);
    void OnButtonRestart(wxCommandEvent &event);
    void OnButtonLock(wxCommandEvent &event);
    void OnButtonClosePrograms(wxCommandEvent &event);
	void OnClose(wxCloseEvent &ev);
	void SendFile(wxCommandEvent &event);

	void SetupGrid();
	void SetupCommandButtons();
	void SetupUpdateField();
	void GridSelectHandler(wxGridEvent &event);
	void AddContactToGrid(char* id, std::string ip);

	void Error(std::string msg);
	void Announcement(std::string msg);
};