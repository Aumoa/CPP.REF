// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <functional>
#include <atomic>

class Thread;
interface IRHICommandBuffer;

class GAME_API RenderThread
{
private:
	static RenderThread* sInstance;

	Thread* RemoteThread = nullptr;
	volatile bool bRunning = false;

	struct Impl_t;
	Impl_t* Impl[2] = {};
	std::atomic<size_t> BufferIndex;
	size_t RenderBufferIndex = 0;

	Task<> Runner = Task<>::ManualTask();
	Task<> Completed = Task<>::CompletedTask();
	IRHICommandBuffer* Context = nullptr;
	std::function<void(IRHICommandBuffer*)> CompletionWork;

public:
	RenderThread();
	~RenderThread() noexcept;

	void EnqueueRenderThreadWork(SObject* Object, std::function<void(IRHICommandBuffer*)> Work);
	Task<> ExecuteWorks(IRHICommandBuffer* InDeviceContext, std::function<void(IRHICommandBuffer*)> InCompletionWork);
	static bool InRenderThread();

public:
	static RenderThread* Get() { return sInstance; }

private:
	void Run();
};