#include "FrameWork.h"
#include <ranges>

FrameWork::~FrameWork()
{
	Stop();
}

void FrameWork::Start(unsigned char logicThreadCount)
{
	Initialize(logicThreadCount);
}

void FrameWork::Stop()
{
	for (auto& thread : logicThreads)
	{
		if (thread.joinable())
		{
			thread.request_stop();
		}
	}

	logicThreads.clear();
}

void FrameWork::Initialize(unsigned char logicThreadCount)
{
	logicThreadCount = std::max(logicThreadCount, static_cast<unsigned char>(1));
	for (unsigned char i = 0; i < logicThreadCount; ++i)
	{
		actors.emplace_back(std::unordered_map<ActorId, std::shared_ptr<Actor>>());
		actorsMutex.emplace_back(std::make_unique<std::shared_mutex>());
		logicThreads.emplace_back([this, i](std::stop_token stopToken) { RunLogicThread(stopToken, i); });
	}
}

void FrameWork::RunLogicThread(std::stop_token stopToken, ThreadIdType threadId)
{
	static constexpr int SLEEP_TIME_MS = 16;
	while (!stopToken.stop_requested())
	{
		PreWakeLogicThread(threadId);
		OnWakeLogicThread(threadId);
		PostWakeLogicThread(threadId);

		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME_MS));
	}
}

void FrameWork::PreWakeLogicThread(const ThreadIdType threadId)
{
	std::shared_lock lock(*actorsMutex[threadId]);
	for (auto& actor : actors[threadId] | std::views::values)
	{
		if (actor != nullptr)
		{
			actor->PreTimer();
		}
	}
}

void FrameWork::OnWakeLogicThread(const ThreadIdType threadId)
{
	std::shared_lock lock(*actorsMutex[threadId]);
	for (auto& actor : actors[threadId] | std::views::values)
	{
		if (actor != nullptr)
		{
			actor->OnTimer();
		}
	}
}

void FrameWork::PostWakeLogicThread(const ThreadIdType threadId)
{
	std::shared_lock lock(*actorsMutex[threadId]);
	for (auto& actor : actors[threadId] | std::views::values)
	{
		if (actor != nullptr)
		{
			actor->PostTimer();
		}
	}
}
