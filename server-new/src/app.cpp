#include "../headers/app.h"
#include "../headers/window.h"

bool App::OnInit() {
    Window* window = new Window("Server", wxPoint(40, 40), wxSize(800, 500));
    window->Show(true);
    return true;
} wxIMPLEMENT_APP(App);