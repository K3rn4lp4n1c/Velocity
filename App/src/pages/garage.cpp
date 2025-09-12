#include "pages/garage.hpp"

namespace garageUI {
    static void display() {
        UI ui;
        ui.print_center("Welcome to the garage");
        refresh();
        getch();
    }
}