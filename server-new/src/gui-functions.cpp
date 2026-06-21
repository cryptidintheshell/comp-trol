void Window::SetupCommandButtons() {
	cmd_panel = new wxPanel(main_panel, wxID_ANY);
	std::vector<wxButton*> buttons;

    std::vector<std::string> btn_labels = { "Shutdown", "Restart", "Lock", "Close programs", "Start", "Send file" };
	std::vector<void(Window::*)(wxCommandEvent&)> buttonEvents = {
	    &Window::OnButtonShutdown,
	    &Window::OnButtonRestart,
	    &Window::OnButtonLock,
	    &Window::OnButtonClosePrograms,
	    &Window::StartServer,
	    &Window::SendFile
	};

	for (size_t i = 0; i < btn_labels.size(); i++) {
		wxButton* btn = new wxButton(cmd_panel, wxID_ANY, btn_labels[i]);
		btn->Bind(wxEVT_BUTTON, buttonEvents[i], this);
		buttons.push_back(btn);
	}

	wxGridSizer* gridSizer = new wxGridSizer(3, 3, 5, 2);
    for (wxButton* btn : buttons) {
        gridSizer->Add(btn, 1, wxALL);
    } cmd_panel->SetSizer(gridSizer);
    
    gridSizer->Fit(cmd_panel);
}

void Window::SetupUpdateField() {
	update_panel = new wxPanel(main_panel, wxID_ANY);
	update_field = new wxTextCtrl(update_panel, wxID_ANY, "", wxDP, wxDS, wxTE_MULTILINE | wxTE_READONLY);
	
	update_panel_sizer = new wxBoxSizer(wxVERTICAL);
	update_panel_sizer->Add(update_field, 1, wxEXPAND);
	update_panel->SetSizer(update_panel_sizer);
}

void Window::SetupGrid() {
	client_panel = new wxPanel(main_panel, wxID_ANY);
	clients_grid = new wxGrid(client_panel, wxID_ANY);
	clients_grid->CreateGrid(0,2); // (row, column)
	clients_grid->SetColLabelValue(0, "ID");
	clients_grid->SetColLabelValue(1, "Address");
	clients_grid->EnableEditing(false);
	clients_grid->HideRowLabels();
	clients_grid->Bind(wxEVT_GRID_SELECT_CELL, &Window::GridSelectHandler, this);

	client_panel_sizer = new wxBoxSizer(wxVERTICAL);
	client_panel_sizer->Add(clients_grid, 1, wxEXPAND);
	client_panel->SetSizer(client_panel_sizer);

	int grid_width = clients_grid->GetSize().GetWidth();
	int id_col = clients_grid->GetColSize(0);
	int addr_col = clients_grid->GetColSize(1);

    clients_grid->SetColSize(1, addr_col + grid_width + (id_col * 2));
	clients_grid->ForceRefresh();
}

void Window::GridSelectHandler(wxGridEvent &event) {
    int selectedRow = event.GetRow();
    current_client = clients_grid->GetCellValue(selectedRow, 0);
    current_socket = client_sockets[selectedRow];
    printf("current socket selected%i\n", current_socket);
    event.Skip();
}

void Window::OnClose(wxCloseEvent &ev) {
	close(server_socket);
	exit(0);
}