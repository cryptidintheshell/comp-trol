void Window::SetupCommandButtons() {
	cmd_panel = new wxPanel(main_panel, wxID_ANY);
    
    // Create the static box to give it a "panel style"
    wxStaticBox* box = new wxStaticBox(cmd_panel, wxID_ANY, "Commands");
    cmd_panel_sizer = new wxStaticBoxSizer(box, wxVERTICAL);

    // Create the grid sizer for the buttons (the "table layout")
    wxGridSizer* gridSizer = new wxGridSizer(2, 3, 5, 5); // 2 rows, 3 columns

    shutdown_button = new wxButton(box, wxID_ANY, "Shutdown");
    restart_button = new wxButton(box, wxID_ANY, "Restart");
    lock_button = new wxButton(box, wxID_ANY, "Lock");
    close_program_button = new wxButton(box, wxID_ANY, "Close programs");
    start_button = new wxButton(box, wxID_ANY, "Start");
    send_file_button = new wxButton(box, wxID_ANY, "Send file");

    shutdown_button->Bind(wxEVT_BUTTON, &Window::OnButtonShutdown, this);
    restart_button->Bind(wxEVT_BUTTON, &Window::OnButtonRestart, this);
    lock_button->Bind(wxEVT_BUTTON, &Window::OnButtonLock, this);
    close_program_button->Bind(wxEVT_BUTTON, &Window::OnButtonClosePrograms, this);
    start_button->Bind(wxEVT_BUTTON, &Window::StartServer, this);
    send_file_button->Bind(wxEVT_BUTTON, &Window::SendFile, this);

    gridSizer->Add(shutdown_button, 1, wxEXPAND | wxALL, 2);
    gridSizer->Add(restart_button, 1, wxEXPAND | wxALL, 2);
    gridSizer->Add(lock_button, 1, wxEXPAND | wxALL, 2);
    gridSizer->Add(close_program_button, 1, wxEXPAND | wxALL, 2);
    gridSizer->Add(start_button, 1, wxEXPAND | wxALL, 2);
    gridSizer->Add(send_file_button, 1, wxEXPAND | wxALL, 2);

    cmd_panel_sizer->Add(gridSizer, 1, wxEXPAND | wxALL, 5);
    cmd_panel->SetSizer(cmd_panel_sizer);
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