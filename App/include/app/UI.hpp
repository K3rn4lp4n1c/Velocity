#pragma once
#include <curses.h>

namespace app {
    inline void initCurses() {
        initscr(); cbreak(); noecho(); keypad(stdscr, TRUE); nodelay(stdscr, TRUE); curs_set(0);
        if (has_colors()) {
            start_color();
            init_pair(1, COLOR_WHITE, COLOR_BLACK);
            init_pair(2, COLOR_BLUE, COLOR_BLACK);
            init_pair(3, COLOR_YELLOW, COLOR_BLACK);
            init_pair(4, COLOR_GREEN, COLOR_BLACK);
            init_pair(5, COLOR_CYAN, COLOR_BLACK);
        }
    }
    inline void shutdownCurses() { endwin(); }
} // namespace app::ui
