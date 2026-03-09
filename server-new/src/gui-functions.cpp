void Window::SetupCommandButtons() {
	pnlCmd = new wxPanel(pnlMain, wxID_ANY);
    
    // Create the static box to give it a "panel style"
    wxStaticBox* box = new wxStaticBox(pnlCmd, wxID_ANY, "Commands");
    szrCmdPanel = new wxStaticBoxSizer(box, wxVERTICAL);

    // Create the grid sizer for the buttons (the "table layout")
    wxGridSizer* gridSizer = new wxGridSizer(2, 3, 5, 5); // 2 rows, 3 columns

    btnShutdown = new wxButton(box, wxID_ANY, "Shutdown");
    btnRestart = new wxButton(box, wxID_ANY, "Restart");
    btnLock = new wxButton(box, wxID_ANY, "Lock");
    btnCloseProgram = new wxButton(box, wxID_ANY, "Close programs");
    btnStart = new wxButton(box, wxID_ANY, "Start");
    btnSendFile = new wxButton(box, wxID_ANY, "Send file");

    btnShutdown->Bind(wxEVT_BUTTON, &Window::OnButtonShutdown, this);
    btnRestart->Bind(wxEVT_BUTTON, &Window::OnButtonRestart, this);
    btnLock->Bind(wxEVT_BUTTON, &Window::OnButtonLock, this);
    btnCloseProgram->Bind(wxEVT_BUTTON, &Window::OnButtonClosePrograms, this);
    btnStart->Bind(wxEVT_BUTTON, &Window::StartServer, this);
    btnSendFile->Bind(wxEVT_BUTTON, &Window::SendFile, this);

    gridSizer->Add(btnShutdown, 1, wxEXPAND | wxALL, 2);
    gridSizer->Add(btnRestart, 1, wxEXPAND | wxALL, 2);
    gridSizer->Add(btnLock, 1, wxEXPAND | wxALL, 2);
    gridSizer->Add(btnCloseProgram, 1, wxEXPAND | wxALL, 2);
    gridSizer->Add(btnStart, 1, wxEXPAND | wxALL, 2);
    gridSizer->Add(btnSendFile, 1, wxEXPAND | wxALL, 2);

    szrCmdPanel->Add(gridSizer, 1, wxEXPAND | wxALL, 5);
    pnlCmd->SetSizer(szrCmdPanel);
}

void Window::SetupUpdateField() {
	pnlUpdate = new wxPanel(pnlMain, wxID_ANY);
	txtctrlUpdateField = new wxTextCtrl(pnlUpdate, wxID_ANY, "", wxDP, wxDS, wxTE_MULTILINE | wxTE_READONLY);
	
	szrUpdatePanel = new wxBoxSizer(wxVERTICAL);
	szrUpdatePanel->Add(txtctrlUpdateField, 1, wxEXPAND);
	pnlUpdate->SetSizer(szrUpdatePanel);
}

void Window::SetupGrid() {
	pnlClient = new wxPanel(pnlMain, wxID_ANY);
	grdClients = new wxGrid(pnlClient, wxID_ANY);
	grdClients->CreateGrid(0,2); // (row, column)
	grdClients->SetColLabelValue(0, "ID");
	grdClients->SetColLabelValue(1, "Address");
	grdClients->EnableEditing(false);
	grdClients->HideRowLabels();
	grdClients->Bind(wxEVT_GRID_SELECT_CELL, &Window::GridSelectHandler, this);

	szrClientPanel = new wxBoxSizer(wxVERTICAL);
	szrClientPanel->Add(grdClients, 1, wxEXPAND);
	pnlClient->SetSizer(szrClientPanel);

	int grid_width = grdClients->GetSize().GetWidth();
	int id_col = grdClients->GetColSize(0);
	int addr_col = grdClients->GetColSize(1);

    grdClients->SetColSize(1, addr_col + grid_width + (id_col * 2));
	grdClients->ForceRefresh();
}

void Window::GridSelectHandler(wxGridEvent &event) {
    int selectedRow = event.GetRow();
    current_client = grdClients->GetCellValue(selectedRow, 0);
    current_socket = client_sockets[selectedRow];
    printf("current socket selected%i\n", current_socket);
    event.Skip();
}

void Window::OnClose(wxCloseEvent &ev) {
	close(server_socket);
	exit(0);
}