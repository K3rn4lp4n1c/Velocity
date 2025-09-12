#include <vector>
#include <string>
#include <curses.h>
#include <stdexcept>

#include "src/pages/home.cpp"
#include "src/pages/garage.cpp"

class CursesSession {
public:
    CursesSession() {
        if (!initscr()) throw std::runtime_error("initscr failed");
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        curs_set(0);
        if (has_colors()) start_color();
    }
    ~CursesSession() { endwin(); }
    CursesSession(const CursesSession&) = delete;
    CursesSession& operator=(const CursesSession&) = delete;
};

struct Inventory {
    std::vector<std::string> cars;
};

int main() {
    CursesSession session;
    struct Inventory { std::vector<std::string> cars; } inventory;

    homeUI::display();
    //garageUI::display();

    return 0;
}