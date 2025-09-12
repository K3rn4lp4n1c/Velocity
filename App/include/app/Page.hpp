#pragma once
#include "Component.hpp"
#include "AppContext.hpp"

namespace app {
	class Router; // fwd

	class Page : public Component {
	public:
		Page(AppContext& ctx, Router& r) : ctx_(ctx), router_(r) {}
		virtual ~Page() = default;

	protected:
		AppContext& ctx_;
		Router& router_;
	};

} // namespace app
