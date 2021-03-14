// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Object.h"

#include "CoreString.h"
#include "TMulticastDelegate.h"
#include "Duration.h"

class CORE_API StepTimer : virtual public Object
{
public:
	using Super = Object;

	using StepTimerCallbackDelegate = TFunction<void(StepTimer*)>;

private:
	const uint64 frequency;
	uint64 standard;

	uint64 lasttime;
	uint64 maxDelta;

	uint64 elapsedTicks;
	uint64 totalTicks;
	uint64 leftOverTicks;

	uint32 frameCount;
	uint32 framesPerSecond;
	uint32 framesThisSecond;
	uint64 secondCounter;

	bool bFixedTimeStep : 1;
	bool bPrintSimplyScope : 1;
	uint64 targetElapsedTicks;

public:
	StepTimer();
	~StepTimer() override;

	TRefPtr<String> ToString() const override;

	void ResetElapsedTime();
	void Tick();

	StepTimerCallbackDelegate StepTimerCallback;

	vs_property_get_auto(uint64, ElapsedTicks, elapsedTicks);
	vs_property_get_auto(Seconds, ElapsedSeconds, TicksToSeconds(elapsedTicks));
	vs_property_get_auto(uint64, TotalTicks, totalTicks);
	vs_property_get_auto(Seconds, TotalSeconds, TicksToSeconds(totalTicks));
	vs_property_get_auto(uint64, TickCount, frameCount);
	vs_property_get_auto(int32, TicksPerSecond, (int32)framesPerSecond);
	vs_property_auto(bool, IsFixedTimeStep, bFixedTimeStep);
	vs_property_auto(uint64, TargetElapsedTicks, targetElapsedTicks);
	vs_property(Seconds, TargetElapsedSeconds);
	vs_property(int32, TargetElapsedTicksPerSecond);
	vs_property_auto(uint64, MaxDeltaTicks, maxDelta);
	vs_property(Seconds, MaxDeltaSeconds);
	vs_property_get_auto(uint64, StandardPerformanceCounter, QueryPerformanceCounter() - standard);

private:
	uint64 SecondsToTicks(Seconds value) const;
	Seconds TicksToSeconds(uint64 value) const;

private:
	static uint64 QueryPerformanceFrequency();
	static uint64 QueryPerformanceCounter();
};