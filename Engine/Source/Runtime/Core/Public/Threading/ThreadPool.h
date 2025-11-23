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

	public:
		template<class TBody>
#if __cpp_lib_move_only_function
		using function_t = std::move_only_function<TBody>;
#else
		using function_t = std::function<TBody>;
#endif

	private:
		static void (*coreclr__QueueUserWorkItem)();
		static void (*coreclr__GetMinThreads)(int32* workerThreads, int32* completionPortThreads);
		static void (*coreclr__GetMaxThreads)(int32* workerThreads, int32* completionPortThreads);
		static void (*coreclr__SetMinThreads)(int32 workerThreads, int32 completionPortThreads);
		static void (*coreclr__SetMaxThreads)(int32 workerThreads, int32 completionPortThreads);

		static size_t NumCompletionPortThreads;

		static Spinlock Lck;
		static std::queue<function_t<void()>> Works;

		static Spinlock DelayedLck;
		static SpinlockConditionVariable DelayedCv;
		static std::multimap<std::chrono::steady_clock::time_point, function_t<void()>> DelayedWorks;

		static void* IO;
		static size_t IOCPWorkers;
		static std::vector<std::thread> Threads;

	private:
		static void static__ThreadPool();

	public:
		static void BindHandle(void* NativeHandle);
		static void UnbindHandle(void* NativeHandle);

		static void QueueUserWorkItem(function_t<void()> InWork);
		static void QueueDelayedUserWorkItem(std::chrono::nanoseconds InDur, function_t<void()> InWork);
		static void QueueSignal();

		static void GetMinThreads(int32* workerThreads, int32* completionPortThreads);
		static void GetMaxThreads(int32* workerThreads, int32* completionPortThreads);
		static void SetMinThreads(int32 workerThreads, int32 completionPortThreads);
		static void SetMaxThreads(int32 workerThreads, int32 completionPortThreads);

	private:
		static void IOCPWorker(size_t Index);
		static void DelayedWorker();
		static void HandleUserWorkItem();
	};
}