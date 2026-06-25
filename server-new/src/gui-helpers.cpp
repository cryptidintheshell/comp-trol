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
