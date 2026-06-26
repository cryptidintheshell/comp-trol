String MainPage(int client_count) {
	String html = "<!DOCTYPE html>";
	html += "<html>";
	html += "<head>";
	html += "<meta charset='utf-8'>";
	html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
	html += "<title>Control</title>";
	html += "<style>";
	html += "* { padding: 0; margin: 0; font-family: arial; }";
	html += "body { height: 100vh; display: flex; justify-content: center; align-items: center; }";
	html += ".container { height: 80%; width: auto; border-radius: 20px; padding: 10px; text-align: center; background: lightgray; }";
	html += ".client-container { margin: 10px; display: grid; grid-template-columns: auto auto; gap: 20px; }";
	html += ".client { padding: 10px; background: limegreen; display: flex; align-items: center; justify-content: space-between; }";
	html += "#disconnected { background: dimgray; }";
	html += ".client-details { text-align: left; }";
	html += ".client-controls { display: flex; justify-content: space-between; }";
	html += ".button { margin-left: 10px; padding: 10px; cursor: pointer; }";
	html += "#shutdown-ctrl { background: red; }";
	html += "#restart-ctrl { background: orange; }";
	html += "#logoff-ctrl { background: lightblue; }";
	html += "#disconnected-ctrl { cursor: not-allowed; background: gray; }";
	html += "</style>";
	html += "</head>";
	html += "<body>";
	html += "<div class='container'>";
	html += "<h1>Command control</h1>";
	html += "<div class='client-container'>";

	// Client 1
	for (int i = 0; i < client_count; ++i) {
		html += "<div class='client' id='client-"+String(i)+"'>";

		html += "<div class='client-details'>";
		html += "<h1>Client " + String(i+1) +"</h1>";
		html += "<h3>PC Name: PC</h3>";
		html += "<h3>IP: " + clients_ip[i] + "</h3>";
		html += "</div>";

		html += "<div class='client-controls'>";
		html += "<div onclick='shutdownClient("+i+")' onclick='' class='button' id='shutdown-ctrl'>Shutdown</div>";
		html += "<div onclick='restartClient("+i+")' onclick='' class='button' id='restart-ctrl'>Restart</div>";
		html += "<div onclick='logoffClient("+i+")' onclick='' class='button' id='logoff-ctrl'>Logoff</div>";
		html += "</div>";

		html += "</div>";
	}  

	html += "</div>"; // close client-container
	html += "</div>"; // close container

	html += "<script>";
    html += "function shutdownClient(id) { fetch('/shutdown?id=' + id); }";
    html += "function restartClient(id) { fetch('/restart?id=' + id); }";
    html += "function logoffClient(id) { fetch('/logoff?id=' + id); }";
    html += "</script>";
    
	html += "</body>";
	html += "</html>";

	return html;
}