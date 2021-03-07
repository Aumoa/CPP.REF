// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Time/StepTimer.h"

#undef interface
#include <Windows.h>
#include "NotSupportedException.h"
#include "Mathematics/Math.h"

StepTimer::StepTimer() : Super()
	, frequency(QueryPerformanceFrequency())
	, standard(0)

	, lasttime(0)
	, maxDelta(0)
	
	, elapsedTicks(0)
	, totalTicks(0)
	, leftOverTicks(0)

	, frameCount(0)
	, framesPerSecond(0)
	, framesThisSecond(0)
	, secondCounter(0)

	, bFixedTimeStep(false)
	, targetElapsedTicks(0)
{
	standard = QueryPerformanceCounter();
}

StepTimer::~StepTimer()
{

}

TRefPtr<String> StepTimer::ToString() const
{
	if (bPrintSimplyScope)
	{
		return String::Format(L"{0}", (Milliseconds)TotalSeconds);
	}
	else
	{
		int32 length = (int32)TotalSeconds.Value;
		bool under = false;
		std::wostringstream builder;

		if (length >= 3600)
		{
			builder << length / 3600 << L"H ";
			length %= 3600;
			under = true;
		}

		if (under || length >= 60)
		{
			builder << length / 60 << L"M ";
			length %= 60;
			under = true;
		}

		builder << length << L" Seconds (" << TicksPerSecond << L" FPS)";

		return builder.str();
	}
}

void StepTimer::ResetElapsedTime()
{
	lasttime = StandardPerformanceCounter;
	framesPerSecond = 0;
	framesThisSecond = 0;
	secondCounter = 0;
}

void StepTimer::Tick()
{
	auto currentTime = StandardPerformanceCounter;
	auto timeDelta = currentTime - lasttime;

	lasttime = currentTime;
	secondCounter += timeDelta;

	if (maxDelta != 0 && timeDelta > maxDelta)
	{
		timeDelta = maxDelta;
	}

	uint32 lastFrameCount = frameCount;

	if (bFixedTimeStep)
	{
		if (Math::Abs((int64)(timeDelta - targetElapsedTicks)) < (int64)TicksPerSecond / 4000)
		{
			timeDelta = targetElapsedTicks;
		}

		leftOverTicks += timeDelta;

		if (leftOverTicks >= targetElapsedTicks)
		{
			elapsedTicks = targetElapsedTicks;
			totalTicks += targetElapsedTicks;
			leftOverTicks -= targetElapsedTicks;
			frameCount += 1;

			StepTimerCallback.Invoke(this);
		}
	}
	else
	{
		elapsedTicks = timeDelta;
		totalTicks += timeDelta;
		leftOverTicks = 0;
		frameCount += 1;

		StepTimerCallback.Invoke(this);
	}

	if (frameCount != lastFrameCount)
	{
		framesThisSecond += 1;
	}

	if (secondCounter >= (uint64)frequency)
	{
		framesPerSecond = framesThisSecond;
		framesThisSecond = 0;
		secondCounter %= frequency;
	}
}

Seconds StepTimer::TargetElapsedSeconds_get() const
{
	return TicksToSeconds(targetElapsedTicks);
}

void StepTimer::TargetElapsedSeconds_set(Seconds value)
{
	targetElapsedTicks = SecondsToTicks(value);
}

int32 StepTimer::TargetElapsedTicksPerSecond_get() const
{
	return (int32)(1.0f / TargetElapsedSeconds.Value);
}

void StepTimer::TargetElapsedTicksPerSecond_set(int32 value)
{
	TargetElapsedSeconds = 1.0f / (float)value;
}

Seconds StepTimer::MaxDeltaSeconds_get() const
{
	return TicksToSeconds(maxDelta);
}

void StepTimer::MaxDeltaSeconds_set(Seconds value)
{
	maxDelta = SecondsToTicks(value);
}

uint64 StepTimer::SecondsToTicks(Seconds value) const
{
	return (uint64)(value.Value * (float)frequency);
}

Seconds StepTimer::TicksToSeconds(uint64 value) const
{
	return (float)value / (float)frequency;
}

uint64 StepTimer::QueryPerformanceFrequency()
{
	uint64 freq;
	if (!::QueryPerformanceFrequency((LARGE_INTEGER*)&freq))
	{
		throw NotSupportedException(L"Performance counter is not supported.");
	}
	return freq;
}

uint64 StepTimer::QueryPerformanceCounter()
{
	uint64 value;
	::QueryPerformanceCounter((LARGE_INTEGER*)&value);
	return value;
}