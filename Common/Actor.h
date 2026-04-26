#pragma once
#include <memory>

class EventBus;

using ActorId = unsigned long;

class FrameWork;

class Actor
{
	friend class FrameWork;

public:
	Actor();
	virtual ~Actor() = default;

private:
	virtual void PreTimer() {}
	virtual void OnTimer() {}
	virtual void PostTimer() {}

private:
	void InitEventBus();

private:
	ActorId actorId{};
	std::shared_ptr<EventBus> eventBus{};
};