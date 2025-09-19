// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Threading/Spinlock.h"
#include "Threading/SpinlockConditionVariable.h"
#include "Platform/PlatformMacros.h"
#include "Action.h"
#include <functional>
#include <chrono>
#include <queue>
#include <map>

extern "C"
{
	PLATFORM_SHARED_EXPORT void Ayla__ThreadPool__HandleUserWorkItem();
}

namespace Ayla
{
	class CORE_API ThreadPool
	{
		friend void ::Ayla__ThreadPool__HandleUserWorkItem();

	private:
		static void (*coreclr__QueueUserWorkItem)();
		static void (*coreclr__GetMaxThreads)(int32* workerThreads, int32* completionPortThreads);

		static size_t NumWorkerThreads;
		static size_t NumCompletionPortThreads;

		static Spinlock Lck;
		static SpinlockConditionVariable Cv;
		static std::queue<Action<>> Works;

		static Spinlock DelayedLck;
		static SpinlockConditionVariable DelayedCv;
		static std::multimap<std::chrono::steady_clock::time_point, Action<>> DelayedWorks;

		static void* IO;
		static size_t Workers;
		static size_t IOCPWorkers;
		static std::vector<std::thread> Threads;

	public:
		static void Initialize(size_t InNumWorkerThreads = 0, size_t InNumCompletionPortThreads = 0);
		static void Shutdown();

		static void BindHandle(void* NativeHandle);
		static void UnbindHandle(void* NativeHandle);

		static void QueueUserWorkItem(Action<> InWork);
		static void QueueDelayedUserWorkItem(std::chrono::nanoseconds InDur, Action<> InWork);
		static void QueueSignal();

		static void GetMinThreads(size_t& OutWorkerThreads, size_t& OutCompletionPortThreads);
		static void GetMaxThreads(size_t& OutWorkerThreads, size_t& OutCompletionPortThreads);

	private:
		static void Worker(size_t Index);
		static void IOCPWorker(size_t Index);
		static void DelayedWorker();
		static void HandleUserWorkItem();
	};
}