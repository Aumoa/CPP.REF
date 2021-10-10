// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <thread>
#include <map>
#include <mutex>
#include <functional>
#include "Threading/Thread.h"

class SThread;
class SEventHandle;

class RenderThread
{
	RenderThread() = delete;

private:
	using WorksDictionary = std::map<size_t, std::function<void()>>;

	struct WaitingThreadWorks
	{
		WorksDictionary Works;
		std::function<void()> CompletedWork;
	};

	struct PendingThreadWork
	{
		WorksDictionary Works;
		std::function<void()> CompletedWork;
		std::shared_ptr<SEventHandle> ExecuteEvent;
		std::shared_ptr<SEventHandle> CompletedEvent;

		void Init();
		void SwapExecute(WaitingThreadWorks& target);
		void RunningWorks_RenderThread();
	};

	struct ThreadInfo
	{
		int64 ThreadId;
		std::thread Thread;
		std::atomic<bool> bRunning;
		std::mutex CriticalSection;

		void Init();
		void Init_RenderThread();
		void WaitToComplete();
		void Worker();
	};

private:
	inline static ThreadInfo _thread;
	inline static WaitingThreadWorks _waitingWorks;
	inline static PendingThreadWork _executingWorks;
	
public:
	static void Init();
	static void Shutdown();
	static void EnqueueRenderThreadWork(size_t workingHash, std::function<void()> work);
	static void ExecuteWorks(std::function<void()> completedWork);
	static void WaitForLastWorks();

	static bool IsInRenderThread()
	{
		static int64 Id = SThread::GetCurrentThread()->GetThreadId();
		return Id == _thread.ThreadId;
	}

private:
	static void InitThreadId();
};