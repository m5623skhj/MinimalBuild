#include "pch.h"
#include "Actor.h"
#include "../Common/EventBus.h"

Actor::Actor()
{
	InitEventBus();
}

void Actor::InitEventBus()
{
	eventBus = std::make_shared<EventBus>();
}
