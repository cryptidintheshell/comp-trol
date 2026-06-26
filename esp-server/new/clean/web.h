String MainPage(int client_count) {
  String html = "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";
  html += "<meta charset='utf-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<meta http-equiv='refresh' content='3'>";
  html += "<title>Command and Control</title>";

  // CSS Styles
  html += "<style>";
  html += "body { margin: 0; padding: 0; height: 100vh; display: flex; flex-direction: column; background-color: whitesmoke; font-family: Arial; justify-content: center; align-items: center; align-content: center; }";
  html += "#main-container { width: fit-content; background-image: linear-gradient(360deg, #00fffb, #0af3f7, #0be1e4, rgb(29, 127, 201, 0.9), rgba(14, 91, 149, 1)); box-shadow: 0 0 10px rgba(0, 0, 0, 0.5); padding: 50px; margin: 10px; color: white; border-radius: 10px; text-align: center; }";
  
  // Client button styles
  html += ".client-button { width: fit-content; background-image: linear-gradient(360deg, #4eff26, #74e44a, #43c226, #379123, #358a21); box-shadow: 0 0 10px rgba(0, 0, 0, 0.4); padding: 10px; margin: 20px 0px; color: white; border-radius: 10px; transition: transform 0.5s; display: flex; justify-content: space-evenly; align-items: center; }";
  html += ".client-button:hover { transform: translateY(-5px); }";
  
  // Shutdown & Restart button styles
  html += ".shutdown-button, .restart-button { padding: 5px; margin: 5px; border-radius: 10px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.4); text-align: center; cursor: pointer; color: white; }";
  html += ".shutdown-button { background-image: linear-gradient(360deg, red, darkred); }";
  html += ".restart-button { background-image: linear-gradient(360deg, #ff8c68, #f4460f, #d73400); }";
  html += "#header { padding: 15px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.4); color: black; border-radius: 40px; text-align: center; }";
  html += "</style>";
  html += "</head>";

  html += "<body>";
  html += "<h1 id='header'>Command and Control</h1>";
  html += "<div id='main-container'>";
  html += "<h1> Client count: " + String(client_count) + "</h1>";
  
  // Loop through each client and generate buttons
  for (int i = 0; i < client_count; i++) {
    html += "<div id='" + String(i) + "' class='client-button'>";
    html += "<p>Client #" + String(i + 1) + "</p>";
    html += "<div class='shutdown-button' onclick='shutdownClient(" + String(i + 1) + ")'>Shatdown</div>";
    html += "<div class='restart-button' onclick='restartClient(" + String(i + 1) + ")'>Restart</div>";
    html += "<div class='restart-button' onclick='logoffClient(" + String(i + 1) + ")'>Logoff</div>";
    html += "</div>";
  }
  html += "</div>";

  html += "<script>";
  html += "function shutdownClient(id) { fetch('/shutdown?id=' + id).then(response => response.text()).then(data => { let e = document.getElementById(data); if (e) e.remove(); }); }";
  html += "function logoffClient(id) { fetch('/logoff?id=' + id).then(response => response.text()).then(data => { let e = document.getElementById(data); if (e) e.remove(); }); }";
  html += "function restartClient(id) { fetch('/restart?id=' + id).then(response => response.text()).then(data => { let e = document.getElementById(data); if (e) e.remove(); }); }";
  html += "</script>";

  html += "</body>";
  html += "</html>";

  return html;
}