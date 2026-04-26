#pragma once
#include "../Common/Context.h"

class EventBus;

class Quest
{
public:
	Quest() = default;
	~Quest() = default;

	void Init(EventBus& eventBus);

private:
	void OnQuestClearRequest(const Context& context, EventBus& eventBus);
	void OnItemCheckResult(const Context& context, EventBus& eventBus);
	void OnItemRemoved(const Context& context, EventBus& eventBus);

private:

};