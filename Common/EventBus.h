#pragma once
#include <functional>

class EventBus
{
public:
	template<typename EventType>
	void Subscribe(std::function<void(const EventType&)> handler)
	{
		handlers[typeid(EventType)].push_back
		(
			[fn](const void* event)
			{
				fn(*static_cast<const EventType*>(event));
			}
		)
	}

	template<typename EventType>
	void Publish(const EventType& event)
	{
		auto it = subscribers.find(typeid(EventType));
		if (it == subscribers.end())
		{
			return;
		}
		
		for (const auto& handler : it->second)
		{
			handler(event);
		}
	}
};