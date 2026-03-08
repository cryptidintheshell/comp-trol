void Window::OnButtonShutdown(wxCommandEvent &event) {
	std::string cmd = "229892";
	if (send(current_socket, cmd.c_str(), cmd.size(), 0) < 0) {
		Error("Failed to send shutdown signal.");
		return;
	} 

	std::string msg = "Shutdown signal was sent to " + current_client;
	Announcement(msg);
}

void Window::OnButtonLock(wxCommandEvent &event) {
	std::string cmd = "837453";
	if (send(current_socket, cmd.c_str(), cmd.size(), 0) < 0) {
		Error("Failed to send lock signal.");
		return;
	} 

	std::string msg = "Lock signal was sent to " + current_client;
	Announcement(msg);
}

void Window::OnButtonRestart(wxCommandEvent &event) {
	std::string cmd = "893234";
	if (send(current_socket, cmd.c_str(), cmd.size(), 0) < 0) {
		Error("Failed to send restart signal.");
		return;
	}

	std::string msg = "Restart signal was sent to " + current_client;
	Announcement(msg + "cmd: " + cmd);
}

void Window::OnButtonClosePrograms(wxCommandEvent &event) {
	std::string cmd = "764853";
	if (send(current_socket, cmd.c_str(), cmd.size(), 0) < 0) {
		Error("Failed to send close programs signal.");
		return;
	} Announcement("Close program signal was sent.");
}

void Window::Announcement(std::string msg) {
	update_field->AppendText("[+] " + msg + "\n");
	printf("[+] %s\n", msg.c_str());
}

void Window::Error(std::string msg) {
	update_field->AppendText("[!] " + msg + "\n");
	printf("[!] %s\n", msg.c_str());
}

void Window::AddContactToGrid(char* id, std::string ip) {
    int rowCount = clients_grid->GetNumberRows();

    clients_grid->AppendRows(1);
	clients_grid->SetCellValue(rowCount, 0, id);
	clients_grid->SetCellValue(rowCount, 1, ip);
}

void Window::SendFile(wxCommandEvent &event) {
	wxFileDialog openFileDialog(this, "Open file", "", "", "Any files (*.*)|*.*|PNG (.png)|*.png", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL) return; 

	std::string fpath = openFileDialog.GetPath().ToStdString();
	std::string fname = openFileDialog.GetFilename().ToStdString();

	struct stat stat_buf;
    int rc = stat(fpath.c_str(), &stat_buf);
    int fsize = (rc == 0) ? stat_buf.st_size : -1;

	send(current_socket, "993123", 6, 0);	// send file transfer signal
	send(current_socket, fname.c_str(), sizeof(fname), 0);
	send(current_socket, std::to_string(fsize).c_str(), sizeof(fsize), 0);

	const size_t buffer_size = 107;	
	char buffer[buffer_size];
	std::ifstream file(fpath, std::ios::binary);
	while (true) {
		file.read(buffer, buffer_size);
	    std::streamsize bytesRead = file.gcount(); // Get number of bytes actually read

	    if (bytesRead > 0) {
	        std::cout << "Sending chunk of size: " << bytesRead << " bytes" << std::endl;
	        send(current_socket, buffer, bytesRead, 0); // Send the chunk to the client
	        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Adjust sleep for rate control
	    }

	    if (file.eof()) break;
  	}

  	file.close();
  	Announcement("File was sent");
}