#pragma once
#include "app/Component.hpp"
#include "app/AppContext.hpp"

namespace comps {

	class StatusBar : public app::Component {
	public:
		explicit StatusBar(app::AppContext& ctx) : ctx_(ctx) {}
		void render(WINDOW* win) override;
	private:
		app::AppContext& ctx_;
	};

} // namespace comps
