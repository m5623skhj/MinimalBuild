#pragma once
#include <unordered_map>
#include <memory>

class Item;
class EventBus;

struct Context;

class Inventory
{
public:
	Inventory() = default;
	~Inventory() = default;

	void Init(EventBus& eventBus);

private:
	void OnItemCheckRequest(const Context& context, EventBus& eventBus);
	void OnItemRemoveRequest(const Context& context, EventBus& eventBus);
	void OnItemAddRequest(const Context& context);


private:
	std::unordered_map<unsigned long long, std::shared_ptr<Item>> inventory;
};
