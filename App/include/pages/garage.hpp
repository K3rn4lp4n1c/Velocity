#pragma once
// Use <ncurses.h> on many systems; <curses.h> also works depending on distro.
#include <curses.h>      // or <curses.h>
#include <stdexcept>
#include <cstring>        // for std::strlen

class NcursesApp {
public:
    NcursesApp() {
        if (!initscr()) throw std::runtime_error("initscr failed");
        cbreak();              // or raw();
        noecho();
        keypad(stdscr, TRUE);
        curs_set(0);           // hide cursor (optional)
        if (has_colors()) {
            start_color();
            // init_pair(1, COLOR_YELLOW, COLOR_BLACK);
        }
    }

    // Print centered on the current screen size
    void print_center(const char* s) {
        int len = static_cast<int>(std::strlen(s));
        int y = LINES / 2;
        int x = (COLS - len) / 2;
        if (x < 0) x = 0;
        mvprintw(y, x, "%s", s);  // SAFE: use format + value
    }

    // Convenience helpers
    void refresh_now() { refresh(); }
    int  wait_key() { return getch(); }

    NcursesApp(const NcursesApp&) = delete;
    NcursesApp& operator=(const NcursesApp&) = delete;

    ~NcursesApp() {
        endwin();  // RAII: always restore terminal
    }
};
