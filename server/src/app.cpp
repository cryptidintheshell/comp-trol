#include "../headers/app.h"
#include "../headers/window.h"

bool App::OnInit() {
    wxInitAllImageHandlers();
    Window* window = new Window("Rat-server", wxPoint(40, 40), wxSize(1000, 600));
    window->Show(true);
    return true;
} wxIMPLEMENT_APP(App);