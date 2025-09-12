#include "components/StatusBar.hpp"
#include <string>

namespace comps {
	void StatusBar::render(WINDOW* win) {
		werase(win); box(win, 0, 0);
		if (has_colors()) wattron(win, COLOR_PAIR(ctx_.theme.colorText));
		mvwprintw(win, 1, 2, "VehicleSim • h:Home  m:Map  q:Quit");
		if (has_colors()) wattroff(win, COLOR_PAIR(ctx_.theme.colorText));
		wrefresh(win);
	}
} // namespace comps
