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

public:
	RHIRenderThread();

	void StopThread();

private:
	void Worker();
};