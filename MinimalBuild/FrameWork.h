#pragma once
#include "Actor.h"
#include <memory>
#include <unordered_map>
#include <thread>
#include <shared_mutex>

using ThreadIdType = unsigned char;

class FrameWork
{
public:
	FrameWork() = default;
	~FrameWork();

	void Start(unsigned char logicThreadCount);
	void Stop();

	void Initialize(unsigned char logicThreadCount);

private:
	void RunLogicThread(std::stop_token stopToken, ThreadIdType threadId);

private:
	void PreWakeLogicThread(const ThreadIdType threadId);
	void OnWakeLogicThread(const ThreadIdType threadId);
	void PostWakeLogicThread(const ThreadIdType threadId);

private:
	std::vector<std::unordered_map<ActorId, std::shared_ptr<Actor>>> actors;
	std::vector<std::unique_ptr<std::shared_mutex>> actorsMutex;
	std::vector<std::jthread> logicThreads;

	std::atomic_int numOfUser{};
};