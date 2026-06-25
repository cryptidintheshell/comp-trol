#include "../headers/window.h"
#include "gui-helpers.cpp"
#include "socket.cpp"
#include "buttons.cpp"
#include "gui-functions.cpp"

Window::Window(const wxString &title, wxPoint wPoint, wxSize wSize)
: wxFrame(NULL, wxID_ANY, title, wPoint, wSize) {
	Bind(wxEVT_CLOSE_WINDOW, &Window::OnClose, this);
	main_panel = new wxPanel(this, wxID_ANY, wxDP, wxSize(400, 200));

	SetupUpdateField();
	SetupGrid();
	SetupCommandButtons();

	vertical_sizer = new wxBoxSizer(wxVERTICAL);
	vertical_sizer->Add(cmd_panel, 0, wxEXPAND);
	vertical_sizer->Add(client_panel, 1, wxEXPAND);

	main_sizer = new wxBoxSizer(wxHORIZONTAL);
	main_sizer->Add(update_panel, 1, wxEXPAND | wxALL, 10);
	main_sizer->Add(vertical_sizer, 0, wxEXPAND | wxALL, 10);
	main_panel->SetSizer(main_sizer);
	main_panel->Fit();
}