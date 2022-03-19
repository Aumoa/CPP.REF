// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>

class Thread;
struct IRHICommandBuffer;

class GAME_API RenderThread
{
	struct Work
	{
		WeakPtr<SObject> Holder;
		std::function<void(IRHICommandBuffer*)> Body;
	};

private:
	static RenderThread* sInstance;

	Thread* _rthread = nullptr;
	bool _running = false;

	std::mutex _lock;
	std::queue<Work> _queuedWorks;
	std::condition_variable _invoke;
	IRHICommandBuffer* _deviceContext = nullptr;
	std::function<void(IRHICommandBuffer*)> _completion;
	TaskCompletionSource<> _taskCompletionSource;


public:
	RenderThread();
	~RenderThread() noexcept;

	void EnqueueRenderThreadWork(SObject* object, std::function<void(IRHICommandBuffer*)> work);
	Task<> ExecuteWorks(IRHICommandBuffer* InDeviceContext, std::function<void(IRHICommandBuffer*)> InCompletionWork);
	static bool InRenderThread();

public:
	static RenderThread* Get() { return sInstance; }

private:
	void Run();
};