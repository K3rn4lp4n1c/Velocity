#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>
#include <string>
#include "Page.hpp"

namespace app {

    enum class Route { Home, Map };

    using PageFactory = std::function<std::unique_ptr<Page>(AppContext&, class Router&)>;

    class Router {
    public:
        explicit Router(AppContext& ctx) : ctx_(ctx) {}
        void registerRoute(Route r, PageFactory fn) { factories_[r] = std::move(fn); }

        void replace(Route r) { stack_.clear(); push(r); }
        void push(Route r) {
            auto it = factories_.find(r);
            if (it != factories_.end()) stack_.push_back(it->second(ctx_, *this));
        }
        void pop() { if (!stack_.empty()) stack_.pop_back(); if (stack_.empty()) ctx_.running = false; }

        Page* current() { return stack_.empty() ? nullptr : stack_.back().get(); }

    private:
        AppContext& ctx_;
        std::unordered_map<Route, PageFactory> factories_;
        std::vector<std::unique_ptr<Page>> stack_;
    };

} // namespace app
