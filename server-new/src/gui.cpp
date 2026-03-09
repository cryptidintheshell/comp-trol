#include "../headers/window.h"
#include "socket.cpp"
#include "buttons.cpp"
#include "gui-functions.cpp"

Window::Window(const wxString &title, wxPoint wPoint, wxSize wSize)
: wxFrame(NULL, wxID_ANY, title, wPoint, wSize) {
	Bind(wxEVT_CLOSE_WINDOW, &Window::OnClose, this);
	pnlMain = new wxPanel(this, wxID_ANY, wxDP, wxSize(400, 200));

	SetupUpdateField();
	SetupGrid();
	SetupCommandButtons();

	szrVertical = new wxBoxSizer(wxVERTICAL);
	szrVertical->Add(pnlCmd, 0, wxEXPAND);
	szrVertical->Add(pnlClient, 1, wxEXPAND);

	szrMain = new wxBoxSizer(wxHORIZONTAL);
	szrMain->Add(pnlUpdate, 1, wxEXPAND | wxALL, 10);
	szrMain->Add(szrVertical, 0, wxEXPAND | wxALL, 10);
	pnlMain->SetSizer(szrMain);
	pnlMain->Fit();
}