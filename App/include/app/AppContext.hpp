#pragma once
#include <string>
#include <cstdint>
#include <curses.h>

namespace app {

	struct Theme {
		short colorFloor = 1, colorWall = 2, colorLandmark = 3, colorVehicle = 4, colorText = 5;
	};

	struct AppContext {
		Theme theme;
		// top-level windows (owned by App)
		WINDOW* mapWin{};
		WINDOW* hudWin{};
		bool running = true;
	};

} // namespace app
