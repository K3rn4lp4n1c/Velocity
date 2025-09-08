#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
#include <curses.h>

struct XY { int x{}, y{}; };

struct Landmark {
    std::string name;
    XY pos;
};

static const std::vector<std::string> MAP = {
    "############################",
    "#..........#...............#",
    "#..####....#....######.....#",
    "#..#..#....#....#....#.....#",
    "#..#..#.........#....#.....#",
    "#..####.........#....#..####",
    "#...............#....#.....#",
    "#######..##############..###",
    "#........#..................",
    "#.######.#.###########.#####",
    "#.#....#.#.#.........#.#...#",
    "#.#.L..#...#....L....#.#...#",
    "#.#....#####.........#.#...#",
    "#.######.....#########.#...#",
    "#.............#.........L..#",
    "############################"
};

static const std::vector<Landmark> LANDMARKS = {
    {"Library",      { 5, 11}},
    {"Cafeteria",    {21, 11}},
    {"Gym",          {22, 14}}
};

bool passable(int x, int y) {
    if (y < 0 || y >= (int)MAP.size()) return false;
    if (x < 0 || x >= (int)MAP[0].size()) return false;
    return MAP[y][x] != '#';
}

int main() {
    // --- curses init ---
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); // non-blocking getch
    curs_set(0);

    if (has_colors()) {
        start_color();
        // map colors
        init_pair(1, COLOR_WHITE, COLOR_BLACK); // floor
        init_pair(2, COLOR_BLUE, COLOR_BLACK); // wall
        init_pair(3, COLOR_YELLOW, COLOR_BLACK); // landmark
        init_pair(4, COLOR_GREEN, COLOR_BLACK); // vehicle
        init_pair(5, COLOR_CYAN, COLOR_BLACK); // HUD text
    }

    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int hudW = std::min(28, std::max(20, cols / 4));
    int mapW = std::max(10, cols - hudW - 1);
    int mapH = std::max(10, rows);

    WINDOW* mapWin = newwin(mapH, mapW, 0, 0);
    WINDOW* hudWin = newwin(mapH, hudW, 0, mapW);

    // vehicle start (on a passable tile)
    XY car{ 2, 2 };
    for (int y = 0; y < (int)MAP.size(); ++y) {
        for (int x = 0; x < (int)MAP[0].size(); ++x) {
            if (MAP[y][x] == '.') { car = { x,y }; y = MAP.size(); break; }
        }
    }

    // camera center on car
    auto clamp = [](int v, int lo, int hi) { return std::max(lo, std::min(v, hi)); };
    auto recenterCamera = [&](int& camX, int& camY) {
        camX = car.x - mapW / 2;
        camY = car.y - mapH / 2;
        camX = clamp(camX, 0, (int)MAP[0].size() - mapW);
        camY = clamp(camY, 0, (int)MAP.size() - mapH);
        };

    int camX = 0, camY = 0;
    recenterCamera(camX, camY);

    // main loop
    bool running = true;
    while (running) {
        // input
        int ch = getch();
        switch (ch) {
        case KEY_UP:    if (passable(car.x, car.y - 1)) car.y--; break;
        case KEY_DOWN:  if (passable(car.x, car.y + 1)) car.y++; break;
        case KEY_LEFT:  if (passable(car.x - 1, car.y)) car.x--; break;
        case KEY_RIGHT: if (passable(car.x + 1, car.y)) car.x++; break;
        case 'q': case 'Q': running = false; break;
        default: break;
        }
        recenterCamera(camX, camY);

        // --- draw map ---
        werase(mapWin);
        box(mapWin, 0, 0);
        for (int sy = 1; sy < mapH - 1; ++sy) {
            int my = camY + (sy - 1);
            if (my < 0 || my >= (int)MAP.size()) continue;
            for (int sx = 1; sx < mapW - 1; ++sx) {
                int mx = camX + (sx - 1);
                if (mx < 0 || mx >= (int)MAP[0].size()) continue;

                chtype chTile;
                short color = 1; // floor
                char t = MAP[my][mx];
                if (t == '#') { chTile = '#'; color = 2; }
                else { chTile = '.'; color = 1; }

                // draw landmarks
                bool isLM = false;
                for (auto const& lm : LANDMARKS) {
                    if (lm.pos.x == mx && lm.pos.y == my) { isLM = true; break; }
                }
                if (isLM) { chTile = 'L'; color = 3; }

                // car on top
                if (mx == car.x && my == car.y) { chTile = 'V'; color = 4; }

                if (has_colors()) wattron(mapWin, COLOR_PAIR(color));
                mvwaddch(mapWin, sy, sx, chTile);
                if (has_colors()) wattroff(mapWin, COLOR_PAIR(color));
            }
        }
        wrefresh(mapWin);

        // --- draw HUD ---
        werase(hudWin);
        box(hudWin, 0, 0);
        if (has_colors()) wattron(hudWin, COLOR_PAIR(5));
        mvwprintw(hudWin, 1, 2, "VehicleSim (ncurses)");
        mvwprintw(hudWin, 3, 2, "Pos: (%d, %d)", car.x, car.y);
        mvwprintw(hudWin, 4, 2, "Cam: (%d, %d)", camX, camY);
        mvwprintw(hudWin, 6, 2, "Controls:");
        mvwprintw(hudWin, 7, 2, "  Arrows: move");
        mvwprintw(hudWin, 8, 2, "  q: quit");
        mvwprintw(hudWin, 10, 2, "Legend:");
        mvwprintw(hudWin, 11, 2, "  #: wall");
        mvwprintw(hudWin, 12, 2, "  .: floor");
        mvwprintw(hudWin, 13, 2, "  L: landmark");
        mvwprintw(hudWin, 14, 2, "  V: vehicle");
        if (has_colors()) wattroff(hudWin, COLOR_PAIR(5));
        wrefresh(hudWin);

        // small frame delay (keeps CPU sane)
        napms(16); // ~60 FPS
    }

    delwin(mapWin);
    delwin(hudWin);
    endwin();
    return 0;
}
