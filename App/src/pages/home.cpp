#include "pages/home.hpp"

namespace homeUI {
    static void test() {
        UI ui;
        ui.print_center("Welcome to Home");
        refresh();
        getch();
    }
    static void display() {
        HomePage Home;
        int ch = getch();
        Home.handleInput(ch);
        Home.update(1);
        Home.render();
        getch();
    }
}
