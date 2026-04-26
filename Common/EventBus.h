#pragma once
#include <functional>
#include "Context.h"

class EventBus
{
public:
    using Handler = std::function<void(const Context&)>;

    void Subscribe(ContextItemType type, Handler handler)
    {
        handlers[type].push_back(handler);
    }

    void Publish(const Context& ctx)
    {
        auto it = handlers.find(ctx.type);
        if (it == handlers.end())
            return;

        for (auto& handler : it->second)
        {
            handler(ctx);
        }
    }

private:
    std::unordered_map<ContextItemType, std::vector<Handler>> handlers;
};