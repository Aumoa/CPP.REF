// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Threading/Thread.h"
#include "RHIRenderThread.gen.h"

SCLASS()
class GAME_API RHIRenderThread : virtual public Object
{
	GENERATED_BODY()

private:
	Thread _thread;

	Spinlock _mutex;
	SpinlockConditionVariable _cv;
	bool _running = false;

	Spinlock _lockWorks;
	std::vector<std::function<void()>> _pendingWorks;
	std::vector<std::function<void()>> _readyWorks;

#if !SHIPPING
	bool _ensureWorks = false;
#endif

public:
	RHIRenderThread();
	virtual ~RHIRenderThread() noexcept override;

	void StopThread(bool join = true);
	bool IsInRenderThread() noexcept;

	void EnqueueWork(std::function<void()> work);
	Task<> ExecuteWorks(std::function<void()> work);

private:
	static RHIRenderThread* sInstance;

public:
	static RHIRenderThread* Get();

private:
	void Worker();
};

inline void EnqueueRenderThreadWork(std::function<void()> work)
{
	auto* instance = RHIRenderThread::Get();
	check(instance);
	instance->EnqueueWork(std::move(work));
}