bool send_all(int socket, const char* buffer, size_t size) {
	size_t total_sent = 0;
	while (total_sent < size) {
		int sent = send(socket, buffer + total_sent, size - total_sent, 0);
		if (sent <= 0) return false;
		total_sent += sent;
	}
	return true;
}

void Window::OnButtonShutdown(wxCommandEvent &event) {
	if (current_socket <= 0) {
		Error("No client selected. Please select a client first before.");
		return;
	}
	std::string cmd = "229892";
	if (send(current_socket, cmd.c_str(), cmd.size(), 0) < 0) {
		Error("Failed to send shutdown signal.");
		return;
	}
	Announcement("Shutdown signal was sent to " + current_client);
}

void Window::OnButtonLock(wxCommandEvent &event) {
	if (current_socket <= 0) {
		Error("No client selected.");
		return;
	}
	std::string cmd = "837453";
	if (send(current_socket, cmd.c_str(), cmd.size(), 0) < 0) {
		Error("Failed to send lock signal.");
		return;
	}
	Announcement("Lock signal was sent to " + current_client);
}

void Window::OnButtonRestart(wxCommandEvent &event) {
	if (current_socket <= 0) {
		Error("No client selected.");
		return;
	}
	std::string cmd = "893234";
	if (send(current_socket, cmd.c_str(), cmd.size(), 0) < 0) {
		Error("Failed to send restart signal.");
		return;
	}
	Announcement("Restart signal was sent to " + current_client);
}

void Window::OnButtonClosePrograms(wxCommandEvent &event) {
	if (current_socket <= 0) {
		Error("No client selected.");
		return;
	}
	std::string cmd = "764853";
	if (send(current_socket, cmd.c_str(), cmd.size(), 0) < 0) {
		Error("Failed to send close programs signal.");
		return;
	}
	Announcement("Close programs signal was sent to " + current_client);
}

void Window::Announcement(std::string msg) {
	txtctrlUpdateField->SetDefaultStyle(wxTextAttr(wxColour(50, 255, 50)));
	txtctrlUpdateField->AppendText("[+] " + msg + "\n");
	printf("[+] %s\n", msg.c_str());
}

void Window::Error(std::string msg) {
	txtctrlUpdateField->SetDefaultStyle(wxTextAttr(wxColour(255, 50, 50)));
	txtctrlUpdateField->AppendText("[!] " + msg + "\n");
	printf("[!] %s\n", msg.c_str());
}

void Window::AddContactToGrid(char* id, std::string ip) {
	int rowCount = grdClients->GetNumberRows();
	grdClients->AppendRows(1);
	grdClients->SetCellValue(rowCount, 0, id);
	grdClients->SetCellValue(rowCount, 1, ip);
	grdClients->AutoSizeColumns();
}

void Window::SendFile(wxCommandEvent &event) {
	if (current_socket <= 0) {
		Error("No client selected.");
		return;
	}

	wxFileDialog openFileDialog(this, "Open file", "", "", "Any files (*.*)|*.*|PNG (.png)|*.png", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL) return;

	std::string fpath = openFileDialog.GetPath().ToStdString();
	std::string fname = openFileDialog.GetFilename().ToStdString();

	struct stat stat_buf;
	int rc = stat(fpath.c_str(), &stat_buf);
	long long fsize = (rc == 0) ? stat_buf.st_size : -1;

	if (fsize < 0) {
		Error("Failed to open or get file size for: " + fname);
		return;
	}

	wxDialog *clientPathDialog = new wxDialog(NULL, wxID_ANY, "Destination path", wxDefaultPosition, wxSize(250, 230));

	wxPanel *panel = new wxPanel(clientPathDialog);
	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);

	wxTextCtrl *txtControl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, 30));
	wxButton *btnOk = new wxButton(panel, wxID_OK, "OK");

	panelSizer->Add(txtControl, 0, wxALL | wxEXPAND, 10);
	panelSizer->Add(btnOk,      0, wxALL | wxCENTER, 5);
	panel->SetSizer(panelSizer);

	wxBoxSizer *dialogSizer = new wxBoxSizer(wxVERTICAL);
	dialogSizer->Add(panel, 1, wxEXPAND);
	clientPathDialog->SetSizer(dialogSizer);

	std::string path = "C:\\Users\\movements\\Documents";	// default fallback
	if (clientPathDialog->ShowModal() == wxID_OK) {
		std::string pathSelected = txtControl->GetValue().ToStdString();
		if (!pathSelected.empty()) path = pathSelected;
	}
	clientPathDialog->Destroy();

	uint32_t pathLength = path.size();
	printf("path: %s\tpath size: %u\n", path.c_str(), pathLength);

	int socket = current_socket;
	std::string client_name = current_client;

	std::thread send_thread([this, socket, fpath, fname, path, pathLength, fsize, client_name]() {
		// 1. Send file command ID
		if (!send_all(socket, "993123", 6)) {
			Error("Failed to send file command ID.");
			return;
		}

		// 2. Brief pause to allow client to enter ReceiveFile mode
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		// 3. Send filename length (4 bytes) + filename
		uint32_t fname_len = fname.size();
		if (!send_all(socket, (char*)&fname_len, sizeof(fname_len))) {
			Error("Failed to send filename length.");
			return;
		}

		// send filename
		if (!send_all(socket, fname.c_str(), fname_len)) {
			Error("Failed to send filename.");
			return;
		}

		// 4. Send path length (4 bytes) + path
		// if (!send_all(socket, (char*)&pathLength, sizeof(pathLength))) {
		// 	Error("Failed to send path length.");
		// 	return;
		// }
		// if (!send_all(socket, path.c_str(), pathLength)) {
		// 	Error("Failed to send path.");
		// 	return;
		// }

		// 5. Send file size (8 bytes)
		uint64_t file_size_val = fsize;
		if (!send_all(socket, (char*)&file_size_val, sizeof(file_size_val))) {
			Error("Failed to send file size.");
			return;
		}

		// 6. Send file content
		std::ifstream file(fpath, std::ios::binary);
		if (!file.is_open()) {
			Error("Failed to open file for reading: " + fpath);
			return;
		}

		const size_t buffer_size = 4096;
		char buffer[buffer_size];
		uint64_t total_sent = 0;

		while (total_sent < file_size_val) {
			file.read(buffer, buffer_size);
			std::streamsize bytesRead = file.gcount();
			if (bytesRead <= 0) break;

			if (!send_all(socket, buffer, bytesRead)) {
				Error("Failed to send file data chunk.");
				file.close();
				return;
			}
			total_sent += bytesRead;
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}

		file.close();
		if (total_sent == file_size_val) {
			Announcement("File " + fname + " was successfully sent to " + client_name + " (" + std::to_string(fsize) + " bytes).");
		} else {
			Error("File transfer to " + client_name + " incomplete. Sent " + std::to_string(total_sent) + " of " + std::to_string(file_size_val) + " bytes.");
		}
	});
	send_thread.detach();
}
