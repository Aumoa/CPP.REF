// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <thread>
#include <map>
#include <mutex>
#include <functional>
#include <future>
#include "Threading/Thread.h"
#include "Misc/CrcHash.h"

interface IRHIDeviceContext;
class Thread;

class RENDERCORE_API RenderThread
{
	RenderThread() = delete;

private:
	using WorksDictionary = std::vector<std::function<void(IRHIDeviceContext*)>>;

	struct WaitingThreadWorks
	{
		WorksDictionary Works;
		std::function<void()> CompletedWork;
	};

	struct PendingThreadWork
	{
		WorksDictionary Works;
		std::function<void()> CompletedWork;
		IRHIDeviceContext* DeviceContext = nullptr;

		void Init();
		void SwapExecute(IRHIDeviceContext* InDeviceContext, WaitingThreadWorks& InTarget);
		void RunningWorks_RenderThread();
	};

	struct ThreadInfo
	{
		int64 ThreadId;
		std::atomic<bool> bRunning;
		std::future<void> ThreadJoin;

		std::mutex WorkerMtx;
		std::condition_variable cvWorker;
		
		std::promise<void> WorkerPromise;
		std::future<void> WorkerFuture;

		void Init();
		void Init_RenderThread();
		void Worker();
	};

private:
	inline static ThreadInfo _Thread;
	inline static WaitingThreadWorks _WaitingWorks;
	inline static PendingThreadWork _ExecutingWorks;
	
public:
	static void Init();
	static void Shutdown();
	static void EnqueueRenderThreadWork(size_t InWorkingHash, std::function<void(IRHIDeviceContext*)> InWorkBody);
	static void ExecuteWorks(IRHIDeviceContext* InDeviceContext, std::function<void()> InCompletionWork, bool bWaitPreviousWork = true);
	static void WaitForLastWorks();

	static bool IsInRenderThread()
	{
		static int64 Id = Thread::GetCurrentThread()->GetThreadId();
		return Id == _Thread.ThreadId;
	}

private:
	template<size_t _N>
	struct StringLiteralHash
	{
		size_t Hs;

		consteval StringLiteralHash(const char(&_Cstr)[_N])
			: Hs(CrcHash::Hash(_Cstr))
		{
		}
	};

public:
	template<StringLiteralHash _String>
	static void EnqueueRenderThreadWork(std::function<void(IRHIDeviceContext*)> Work)
	{
		EnqueueRenderThreadWork(_String.Hs, std::move(Work));
	}
};