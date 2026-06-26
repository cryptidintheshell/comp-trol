#include "../headers/window.h"

void Window::SetupCommandButtons() {
    pnlCmd = new wxPanel(pnlMain, wxID_ANY);
    pnlCmd->SetBackgroundColour(wxColour(45, 45, 48));
    
    wxStaticBox* box = new wxStaticBox(pnlCmd, wxID_ANY, "Remote Control Commands", wxDP, wxDS, wxALIGN_CENTRE_HORIZONTAL);
    box->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    box->SetForegroundColour(*wxWHITE);
    szrCmdPanel = new wxStaticBoxSizer(box, wxVERTICAL);

    wxGridSizer* gridSizer = new wxGridSizer(2, 3, 10, 10);

    wxFont btnFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

    btnShutdown = new wxButton(box, wxID_ANY, "Shutdown");
    btnRestart = new wxButton(box, wxID_ANY, "Restart");
    btnLock = new wxButton(box, wxID_ANY, "Lock");
    btnCloseProgram = new wxButton(box, wxID_ANY, "Close Programs");
    btnStart = new wxButton(box, wxID_ANY, "Start Server");
    btnSendFile = new wxButton(box, wxID_ANY, "Send File");

    wxButton* btns[] = {btnShutdown, btnRestart, btnLock, btnCloseProgram, btnStart, btnSendFile};
    for (wxButton* btn : btns) {
        btn->SetFont(btnFont);
        btn->SetMinSize(wxSize(120, 45));
    }

    // Modern color scheme for buttons
    btnStart->SetBackgroundColour(wxColour(46, 139, 87)); // Sea Green
    btnStart->SetForegroundColour(*wxWHITE);
    btnShutdown->SetBackgroundColour(wxColour(178, 34, 34)); // Firebrick Red
    btnShutdown->SetForegroundColour(*wxWHITE);
    
    // Set darker shades for others to match dark mode
    wxColour darkBtn(60, 60, 60);
    btnRestart->SetBackgroundColour(darkBtn); btnRestart->SetForegroundColour(*wxWHITE);
    btnLock->SetBackgroundColour(darkBtn); btnLock->SetForegroundColour(*wxWHITE);
    btnCloseProgram->SetBackgroundColour(darkBtn); btnCloseProgram->SetForegroundColour(*wxWHITE);
    btnSendFile->SetBackgroundColour(darkBtn); btnSendFile->SetForegroundColour(*wxWHITE);

    btnShutdown->Bind(wxEVT_BUTTON, &Window::OnButtonShutdown, this);
    btnRestart->Bind(wxEVT_BUTTON, &Window::OnButtonRestart, this);
    btnLock->Bind(wxEVT_BUTTON, &Window::OnButtonLock, this);
    btnCloseProgram->Bind(wxEVT_BUTTON, &Window::OnButtonClosePrograms, this);
    btnStart->Bind(wxEVT_BUTTON, &Window::StartServer, this);
    btnSendFile->Bind(wxEVT_BUTTON, &Window::SendFile, this);

    gridSizer->Add(btnShutdown, 1, wxEXPAND);
    gridSizer->Add(btnRestart, 1, wxEXPAND);
    gridSizer->Add(btnLock, 1, wxEXPAND);
    gridSizer->Add(btnCloseProgram, 1, wxEXPAND);
    gridSizer->Add(btnStart, 1, wxEXPAND);
    gridSizer->Add(btnSendFile, 1, wxEXPAND);

    szrCmdPanel->Add(gridSizer, 1, wxEXPAND | wxALL, 10);
    pnlCmd->SetSizer(szrCmdPanel);
}

void Window::SetupUpdateField() {
    pnlUpdate = new wxPanel(pnlMain, wxID_ANY);
    pnlUpdate->SetBackgroundColour(wxColour(45, 45, 48));
    
    wxStaticBox* box = new wxStaticBox(pnlUpdate, wxID_ANY, "System Logs", wxDP, wxDS, wxALIGN_CENTRE_HORIZONTAL);
    box->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    box->SetForegroundColour(*wxWHITE);
    wxStaticBoxSizer* boxSizer = new wxStaticBoxSizer(box, wxVERTICAL);

    txtctrlUpdateField = new wxTextCtrl(box, wxID_ANY, "", wxDP, wxDS, wxTE_MULTILINE | wxTE_READONLY | wxNO_BORDER);
    txtctrlUpdateField->SetBackgroundColour(wxColour(30, 30, 30));
    txtctrlUpdateField->SetForegroundColour(wxColour(0, 255, 65)); // Pure Matrix Green
    txtctrlUpdateField->SetFont(wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    
    boxSizer->Add(txtctrlUpdateField, 1, wxEXPAND | wxALL, 5);
    
    szrUpdatePanel = new wxBoxSizer(wxVERTICAL);
    szrUpdatePanel->Add(boxSizer, 1, wxEXPAND);
    pnlUpdate->SetSizer(szrUpdatePanel);
}

void Window::SetupCardsPanel() {
    pnlCards = new wxScrolledWindow(pnlMain, wxID_ANY, wxDP, wxSize(-1, 150));
    pnlCards->SetBackgroundColour(wxColour(35, 35, 38));
    pnlCards->SetScrollRate(5, 0);  // horizontal scroll only

    boxCards = new wxStaticBox(pnlCards, wxID_ANY, "Clients");
    boxCards->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    boxCards->SetForegroundColour(*wxWHITE);

    szrCardsInner = new wxWrapSizer(wxHORIZONTAL);  // cards go in here

    szrBoxCards = new wxStaticBoxSizer(boxCards, wxVERTICAL);
    szrBoxCards->Add(szrCardsInner, 1, wxEXPAND | wxALL, 5);

    szrCards = new wxBoxSizer(wxVERTICAL);
    szrCards->Add(szrBoxCards, 1, wxEXPAND | wxALL, 5);
    pnlCards->SetSizer(szrCards);
}

void Window::AddClientCard(const std::string& name, const std::string& ip, int socket) {
    wxPanel* card = new wxPanel(boxCards, wxID_ANY, wxDP, wxSize(120, 130), wxBORDER_RAISED);
    card->SetBackgroundColour(wxColour(60, 60, 65));

    wxBoxSizer* cardSizer = new wxBoxSizer(wxVERTICAL);

    // Try to load icon
    wxImage img; // No image handler for type 15 defined.
    wxStaticBitmap* bmp;
    if (img.LoadFile("images/icon.png", wxBITMAP_TYPE_PNG)) {
        bmp = new wxStaticBitmap(card, wxID_ANY, wxBitmap(img.Scale(48, 48, wxIMAGE_QUALITY_HIGH)));
    } else {
        bmp = new wxStaticBitmap(card, wxID_ANY, wxArtProvider::GetBitmap(wxART_HARDDISK, wxART_OTHER, wxSize(48, 48)));
    }

    wxStaticText* lblName = new wxStaticText(card, wxID_ANY, name, wxDP, wxDS, wxALIGN_CENTER);
    lblName->SetForegroundColour(*wxWHITE);
    lblName->SetFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    wxStaticText* lblIp = new wxStaticText(card, wxID_ANY, ip, wxDP, wxDS, wxALIGN_CENTER);
    lblIp->SetForegroundColour(wxColour(200, 200, 200));
    lblIp->SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    cardSizer->Add(bmp, 0, wxALIGN_CENTER | wxTOP, 10);
    cardSizer->Add(lblName, 0, wxALIGN_CENTER | wxTOP, 5);
    cardSizer->Add(lblIp, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 5);

    card->SetSizer(cardSizer);

    ClientCard cc;
    cc.pnlContainer = card;
    cc.bmpIcon = bmp;
    cc.stxtComputerName = lblName;
    cc.stxtComputerIp = lblIp;
    cc.socket = socket;

    client_cards.push_back(cc);
    szrCardsInner->Add(card, 0, wxALL, 5);
    
    pnlCards->Layout();
    pnlCards->FitInside();
}

void Window::RemoveClientCard(int socket) {
    for (auto it = client_cards.begin(); it != client_cards.end(); ++it) {
        if (it->socket == socket) {
            it->pnlContainer->Destroy();
            client_cards.erase(it);
            szrCardsInner->Layout();
            pnlCards->FitInside();
            return;
        }
    }
    Error("No client card found for socket " + std::to_string(socket));
}

void Window::SetupGrid() {
    pnlClient = new wxPanel(pnlMain, wxID_ANY);
    pnlClient->SetBackgroundColour(wxColour(45, 45, 48));
    
    wxStaticBox* box = new wxStaticBox(pnlClient, wxID_ANY, "Connected Clients", wxDP, wxDS, wxALIGN_CENTRE_HORIZONTAL);
    box->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    box->SetForegroundColour(*wxWHITE);
    wxStaticBoxSizer* boxSizer = new wxStaticBoxSizer(box, wxVERTICAL);

    grdClients = new wxGrid(box, wxID_ANY);
    grdClients->CreateGrid(0, 2);
    grdClients->SetColLabelValue(0, "Username");
    grdClients->SetColLabelValue(1, "IP Address");
    grdClients->EnableEditing(false);
    grdClients->HideRowLabels();
    grdClients->SetSelectionMode(wxGrid::wxGridSelectionModes::wxGridSelectRows);
    
    // Dark mode grid styling
    grdClients->SetDefaultCellBackgroundColour(wxColour(50, 50, 50));
    grdClients->SetDefaultCellTextColour(*wxWHITE);
    grdClients->SetGridLineColour(wxColour(70, 70, 70));
    grdClients->SetLabelBackgroundColour(wxColour(40, 40, 40));
    grdClients->SetLabelTextColour(*wxWHITE);
    grdClients->SetSelectionBackground(wxColour(0, 120, 215)); // Windows blue selection
    
    grdClients->SetLabelFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    grdClients->SetDefaultCellAlignment(wxALIGN_CENTER, wxALIGN_CENTER);
    
    grdClients->Bind(wxEVT_GRID_SELECT_CELL, &Window::GridSelectHandler, this);

    boxSizer->Add(grdClients, 1, wxEXPAND | wxALL, 5);

    szrClientPanel = new wxBoxSizer(wxVERTICAL);
    szrClientPanel->Add(boxSizer, 1, wxEXPAND);
    pnlClient->SetSizer(szrClientPanel);

    grdClients->SetColSize(0, 100);
    grdClients->Bind(wxEVT_SIZE, [this](wxSizeEvent& e) {
        int remaining = grdClients->GetClientSize().GetWidth() - 100;
        if (remaining > 0) grdClients->SetColSize(1, remaining);
        e.Skip();
    });
}

void Window::GridSelectHandler(wxGridEvent &event) {
    int selectedRow = event.GetRow();
    if (selectedRow >= 0 && selectedRow < (int)client_sockets.size()) {
        current_client = grdClients->GetCellValue(selectedRow, 0);
        current_socket = client_sockets[selectedRow];
        printf("Selected client: %s (Socket: %d)\n", current_client.c_str(), current_socket);
        SetStatusText("Selected Client: " + current_client + " (" + grdClients->GetCellValue(selectedRow, 1) + ")");
    }
    event.Skip();
}

void Window::OnClose(wxCloseEvent &ev) {
    if (server_socket > 0) {
        close(server_socket);
    }
    exit(0);
}