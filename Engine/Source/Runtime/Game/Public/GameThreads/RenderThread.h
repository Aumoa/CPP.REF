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
	static int64 _threadId;
	static std::thread _thread;
	static std::map<size_t, std::function<void()>> _works;
	static std::map<size_t, std::function<void()>> _pendingWorks;
	static std::mutex _locker;
	static std::shared_ptr<SEventHandle> _executeEvent;
	static std::atomic<bool> _bSwitch;
	
public:
	static void Init();
	static void Shutdown();
	static void EnqueueRenderThreadWork(size_t workingHash, std::function<void()> work);
	static void ExecuteWorks();

	static bool IsInRenderThread()
	{
		static int64 Id = SThread::GetCurrentThread()->GetThreadId();
		return Id == _threadId;
	}

private:
	static void InitThreadId();
	static void Worker();
};