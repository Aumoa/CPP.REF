// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Threading/Thread.h"
#include "RHIRenderThread.generated.h"

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

	void StopThread(bool join = true);

	void EnqueueWork(std::function<void()> work);
	Task<> ExecuteWorks(std::function<void()> work);

private:
	void Worker();
};