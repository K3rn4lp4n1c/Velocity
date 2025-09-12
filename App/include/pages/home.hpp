#pragma once
#include <curses.h>

class UI {
public:
    UI() = default;
    void print_center(std::string s) const {
        int rows = 0, cols = 0;
        getmaxyx(stdscr, rows, cols);
        int len = static_cast<int>(s.size());
        int y = rows / 2;
        int x = (cols - len) / 2;
        if (x < 0) x = 0;
        mvprintw(y, x, "%.*s", len, s.data());
    }
    UI(const UI&) = delete;
    UI& operator=(const UI&) = delete;
    UI(UI&&) = default;
    UI& operator=(UI&&) = default;
};

#include <string>
#include <vector>
#include <cstring> 
#include <algorithm> 

class HomePage {
public:
    HomePage()
        : title_("VehicleSim"), subtitle_("Home"), authors_("By: You & Muhammad") {
    }

    ~HomePage() {
        if (mapWin_) delwin(mapWin_);
        if (hudWin_) delwin(hudWin_);
    }

    void print_center(std::string s) const {
        int rows = 0, cols = 0;
        getmaxyx(stdscr, rows, cols);
        int len = static_cast<int>(s.size());
        int y = rows / 2;
        int x = (cols - len) / 2;
        if (x < 0) x = 0;
        mvprintw(y, x, "%.*s", len, s.data());
    }

    // Optional: local UI feel (doesn't trigger routing)
    void handleInput(int ch) {
        switch (ch) {
        case KEY_UP:   if (sel_ > 0) sel_--; break;
        case KEY_DOWN: if (sel_ + 1 < (int)items_.size()) sel_++; break;
        case 'c': case 'C': showControls_ = !showControls_; break;
        default: break; // routing keys handled by App if desired
        }
    }

    void update(double dt_seconds) {
        blink_ += dt_seconds;
        if (blink_ > 10.0) blink_ = 0.0;   // keep bounded
    }

    // Creates/refreshes windows as needed and draws the page.
    // Assumes ncurses is already initialized by the App (initscr(), etc.).
    void render() {
        ensureWindows_();
        drawMapPanel_();
        drawHudPanel_();
    }

private:
    // UI content
    std::string title_, subtitle_, authors_;
    std::vector<std::string> items_{ "Map", "Controls", "Quit" };
    int sel_ = 0;
    bool showControls_ = false;
    double blink_ = 0.0;

    // Windows & state
    WINDOW* mapWin_ = nullptr;
    WINDOW* hudWin_ = nullptr;
    int lastRows_ = 0, lastCols_ = 0;
    bool colorsInit_ = false;

    void ensureWindows_() {
        int rows = 0, cols = 0;
        getmaxyx(stdscr, rows, cols);

        // Lazily initialize colors once.
        if (!colorsInit_ && has_colors()) {
            start_color();
            init_pair(1, COLOR_WHITE, COLOR_BLACK); // text/floor
            init_pair(2, COLOR_BLUE, COLOR_BLACK); // lines
            init_pair(3, COLOR_YELLOW, COLOR_BLACK); // accents
            colorsInit_ = true;
        }

        // Recreate on first run or if the terminal size changed.
        if (!mapWin_ || !hudWin_ || rows != lastRows_ || cols != lastCols_) {
            if (mapWin_) { delwin(mapWin_); mapWin_ = nullptr; }
            if (hudWin_) { delwin(hudWin_); hudWin_ = nullptr; }

            lastRows_ = rows; lastCols_ = cols;
            int hudW = std::max(22, cols / 4);
            int mapW = std::max(10, cols - hudW);

            mapWin_ = newwin(rows, mapW, 0, 0);
            hudWin_ = newwin(rows, hudW, 0, mapW);
        }
    }

    void drawMapPanel_() {
        werase(mapWin_);
        box(mapWin_, 0, 0);

        int h = getmaxy(mapWin_), w = getmaxx(mapWin_);
        auto centerX = [&](const char* s) { return std::max(2, (w - (int)std::strlen(s)) / 2); };

        // Title / subtitle
        mvwprintw(mapWin_, 1, centerX(title_.c_str()), "%s", title_.c_str());
        mvwprintw(mapWin_, 2, centerX(subtitle_.c_str()), "%s", subtitle_.c_str());

        // Simple backdrop “roads”
        if (colorsInit_) wattron(mapWin_, COLOR_PAIR(2));
        for (int x = 2; x < w - 2; ++x) mvwaddch(mapWin_, h / 2, x, '-');
        for (int y = 3; y < h - 2; y += 2) mvwaddch(mapWin_, y, w / 2, '|');
        if (colorsInit_) wattroff(mapWin_, COLOR_PAIR(2));

        // Blinking hint
        if (((int)(blink_ * 2.0)) % 2 == 0) {
            const char* hint = "Use Up/Down • Press C to toggle Controls";
            mvwprintw(mapWin_, h - 2, centerX(hint), "%s", hint);
        }

        wrefresh(mapWin_);
    }

    void drawHudPanel_() {
        werase(hudWin_);
        box(hudWin_, 0, 0);

        if (colorsInit_) wattron(hudWin_, COLOR_PAIR(3));
        mvwprintw(hudWin_, 1, 2, "Menu");
        if (colorsInit_) wattroff(hudWin_, COLOR_PAIR(3));

        for (int i = 0; i < (int)items_.size(); ++i) {
            if (i == sel_) wattron(hudWin_, A_REVERSE);
            mvwprintw(hudWin_, 3 + i, 4, "%s", items_[i].c_str());
            if (i == sel_) wattroff(hudWin_, A_REVERSE);
        }

        int y = 8;
        if (showControls_) {
            mvwprintw(hudWin_, y++, 2, "Controls:");
            mvwprintw(hudWin_, y++, 4, "Up/Down : Move selection");
            mvwprintw(hudWin_, y++, 4, "C       : Toggle controls");
            mvwprintw(hudWin_, y++, 4, "Q / App : Routing handled by App");
        }
        else {
            mvwprintw(hudWin_, y, 2, "Tip: Press 'C' to view controls");
        }

        // Footer
        mvwprintw(hudWin_, getmaxy(hudWin_) - 2, 2, "%s", authors_.c_str());

        wrefresh(hudWin_);
    }
};