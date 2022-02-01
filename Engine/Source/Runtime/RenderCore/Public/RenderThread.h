// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <functional>
#include <atomic>

class Thread;
interface IRHIDeviceContext;

class RENDERCORE_API RenderThread
{
private:
	static RenderThread* sInstance;

	Thread* RemoteThread = nullptr;
	volatile bool bRunning = false;

	struct Impl_t;
	Impl_t* Impl[2] = {};
	std::atomic<size_t> BufferIndex;

	Task<> Runner = Task<>::ManualTask();
	Task<> Completed = Task<>::CompletedTask();

public:
	RenderThread();
	~RenderThread() noexcept;

	void EnqueueRenderThreadWork(std::function<void(IRHIDeviceContext*)> Work);
	Task<> ExecuteWorks(IRHIDeviceContext* InDeviceContext, std::function<void(IRHIDeviceContext*)> InCompletionWork);
	static bool InRenderThread();

public:
	static RenderThread* Get() { return sInstance; }

private:
	void Run();
};