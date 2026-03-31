#include "../headers/window.h"
#include "socket.cpp"
#include "buttons.cpp"
#include "gui-functions.cpp"

Window::Window(const wxString &title, wxPoint wPoint, wxSize wSize)
: wxFrame(NULL, wxID_ANY, title, wPoint, wSize) {
	Bind(wxEVT_CLOSE_WINDOW, &Window::OnClose, this);
	pnlMain = new wxPanel(this, wxID_ANY);
	pnlMain->SetBackgroundColour(wxColour(45, 45, 48)); // Dark Grey background

	SetupUpdateField();
	SetupGrid();
	SetupCommandButtons();
	SetupCardsPanel();

	wxBoxSizer* szrLogs = new wxBoxSizer(wxVERTICAL);
	szrLogs->Add(pnlCards, 0, wxEXPAND | wxBOTTOM, 10);
	szrLogs->Add(pnlUpdate, 1, wxEXPAND);

	szrVertical = new wxBoxSizer(wxVERTICAL);
	szrVertical->Add(pnlClient, 1, wxEXPAND | wxALL, 5);
	szrVertical->Add(pnlCmd, 0, wxEXPAND | wxALL, 5);

	szrMain = new wxBoxSizer(wxHORIZONTAL);
	szrMain->Add(szrLogs, 1, wxEXPAND | wxALL, 10);
	szrMain->Add(szrVertical, 1, wxEXPAND | wxALL, 10);
	
	pnlMain->SetSizer(szrMain);
	
	wxBoxSizer* frameSizer = new wxBoxSizer(wxVERTICAL);
	frameSizer->Add(pnlMain, 1, wxEXPAND);
	this->SetSizer(frameSizer);

	CreateStatusBar();
	SetStatusText("Server Stopped");
}