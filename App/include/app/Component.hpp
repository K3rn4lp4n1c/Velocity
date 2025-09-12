#pragma once
#include <curses.h>

namespace app {

	class Component {
	public:
		virtual ~Component() = default;
		virtual void update(double dt) {};
		virtual void render(WINDOW* win) = 0;             
		virtual bool handleInput(int ch) { return false; }
	};

} // namespace app
