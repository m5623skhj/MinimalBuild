// Inventory.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Inventory.h"
#include "../Common/EventBus.h"

// TODO: 라이브러리 함수의 예제입니다.
void fnInventory()
{
}

void Inventory::Init(EventBus& eventBus)
{
    eventBus.Subscribe(0, [this, &eventBus](const Context& context)
        {
            OnItemCheckRequest(context, eventBus);
        });

    eventBus.Subscribe(1,[this, &eventBus](const Context& context)
        {
            OnItemRemoveRequest(context, eventBus);
        });

    eventBus.Subscribe(2, [this](const Context& context)
        {
            OnItemAddRequest(context);
        });
}

void Inventory::OnItemCheckRequest(const Context& context, EventBus& eventBus)
{
	Context sendContext(7, context.data);
	eventBus.Publish(sendContext);
}

void Inventory::OnItemRemoveRequest(const Context& context, EventBus& eventBus)
{
	Context sendContext(4, context.data);
	eventBus.Publish(sendContext);
}

void Inventory::OnItemAddRequest(const Context& context)
{

}
