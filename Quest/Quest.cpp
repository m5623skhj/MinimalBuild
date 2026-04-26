// Quest.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "pch.h"
#include "Quest.h"
#include "../Common/EventBus.h"

void Quest::Init(EventBus& eventBus)
{
    eventBus.Subscribe(3, [this, &eventBus](const Context& context)
        {
            OnQuestClearRequest(context, eventBus);
        });

    eventBus.Subscribe(2, [this, &eventBus](const Context& context)
        {
            OnItemCheckResult(context, eventBus);
        });

    eventBus.Subscribe(4, [this, &eventBus](const Context& context)
        {
            OnItemRemoved(context, eventBus);
        });
}

void Quest::OnQuestClearRequest(const Context& context, EventBus& eventBus)
{

}

void Quest::OnItemCheckResult(const Context& context, EventBus& eventBus)
{

}

void Quest::OnItemRemoved(const Context& context, EventBus& eventBus)
{

}
