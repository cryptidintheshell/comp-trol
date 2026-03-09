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

class ClientComputer {
	wxPanel* pnlContainer;
	wxStaticText* stxtComputerName;
	wxStaticText* stxtComputerIp;

	int client_socket;
	std::string client_name;
};

class Window : public wxFrame {
	
	wxButton* btnStart;
	wxButton* btnShutdown;
	wxButton* btnRestart;
	wxButton* btnLock;
	wxButton* btnCloseProgram;
	wxButton* btnSendFile;

	wxTextCtrl* txtctrlUpdateField;

	wxPanel* pnlMain;
	wxPanel* pnlButtons;
	wxPanel* pnlCmd;
	wxPanel* pnlUpdate;
	wxPanel* pnlClient;

	wxBoxSizer* szrMain;
	wxBoxSizer* szrButtonsPanel;
	wxBoxSizer* szrUpdatePanel;
	wxStaticBoxSizer* szrCmdPanel;
	wxBoxSizer* szrClientPanel;
	wxBoxSizer* szrVertical;

	wxGrid* grdClients;

	std::vector<ClientComputer> clients_computer;

	std::vector<int> client_sockets;
 	std::vector<std::string> client_address;
 	int client_count = 0;
 	
 	// variables for client selected
 	int current_socket = -1;
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