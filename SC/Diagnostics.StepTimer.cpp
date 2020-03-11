using namespace SC;
using namespace SC::Diagnostics;

using namespace std;
using namespace std::chrono;

inline namespace
{
	using clock = high_resolution_clock;
}

StepTimer::StepTimer()
{
	nanoStandard = clock::now().time_since_epoch().count();
}

String StepTimer::ToString()
{
	int length = ( int )TotalSeconds;
	wstringstream wss;
	bool under = false;

	if ( length >= 3600 )
	{
		wss << length / 3600 << L"H ";
		length %= 3600;
		under = true;
	}

	if ( under || length >= 60 )
	{
		wss << length / 60 << L"M ";
		length %= 60;
		under = true;
	}

	wss << length << L" Seconds (";
	wss << TicksPerSecond << L" FPS)";

	return wss.str();
}

void StepTimer::ResetElapsedTime()
{
	nanoLasttime = GetNanos();

	leftOverTicks = 0;
	framesPerSecond = 0;
	framesThisSecond = 0;
	nanoSecondCounter = 0;
}

void StepTimer::Tick( StepTimerCallbackDelegate updateCallback )
{
	auto currentTime = GetNanos();
	auto timeDelta = currentTime - nanoLasttime;

	nanoLasttime = currentTime;
	nanoSecondCounter += timeDelta;

	if ( nanoMaxDelta != 0 && timeDelta > nanoMaxDelta )
	{
		timeDelta = nanoMaxDelta;
	}

	uint32 lastFrameCount = frameCount;

	if ( isFixedTimeStep )
	{
		if ( abs( ( int64 )( timeDelta - targetElapsedTicks ) ) < ( int64 )TicksPerSecond / 4000 )
		{
			timeDelta = targetElapsedTicks;
		}

		leftOverTicks += timeDelta;

		if ( leftOverTicks >= targetElapsedTicks )
		{
			elapsedTicks = targetElapsedTicks;
			totalTicks += targetElapsedTicks;
			leftOverTicks -= targetElapsedTicks;
			frameCount += 1;

			if ( updateCallback.IsValid )
			{
				updateCallback( this );
			}
		}
	}
	else
	{
		elapsedTicks = timeDelta;
		totalTicks += timeDelta;
		leftOverTicks = 0;
		frameCount += 1;

		if ( updateCallback.IsValid )
		{
			updateCallback( this );
		}
	}

	if ( frameCount != lastFrameCount )
	{
		framesThisSecond += 1;
	}

	if ( nanoSecondCounter >= ( uint64 )frequency )
	{
		framesPerSecond = framesThisSecond;
		framesThisSecond = 0;
		nanoSecondCounter %= frequency;
	}
}

void StepTimer::Tick( function<void()> updateCallback )
{
	auto currentTime = GetNanos();
	auto timeDelta = currentTime - nanoLasttime;

	nanoLasttime = currentTime;
	nanoSecondCounter += timeDelta;

	if ( nanoMaxDelta != 0 && timeDelta > nanoMaxDelta )
	{
		timeDelta = nanoMaxDelta;
	}

	uint32 lastFrameCount = frameCount;

	if ( isFixedTimeStep )
	{
		if ( abs( ( int64 )( timeDelta - targetElapsedTicks ) ) < ( int64 )TicksPerSecond / 4000 )
		{
			timeDelta = targetElapsedTicks;
		}

		leftOverTicks += timeDelta;

		if ( leftOverTicks >= targetElapsedTicks )
		{
			elapsedTicks = targetElapsedTicks;
			totalTicks += targetElapsedTicks;
			leftOverTicks -= targetElapsedTicks;
			frameCount += 1;

			if ( updateCallback )
			{
				updateCallback();
			}
		}
	}
	else
	{
		elapsedTicks = timeDelta;
		totalTicks += timeDelta;
		leftOverTicks = 0;
		frameCount += 1;

		if ( updateCallback )
		{
			updateCallback();
		}
	}

	if ( frameCount != lastFrameCount )
	{
		framesThisSecond += 1;
	}

	if ( nanoSecondCounter >= ( uint64 )frequency )
	{
		framesPerSecond = framesThisSecond;
		framesThisSecond = 0;
		nanoSecondCounter %= frequency;
	}
}

uint64 StepTimer::ElapsedTicks_get() const
{
	return elapsedTicks;
}

double StepTimer::ElapsedSeconds_get() const
{
	return TicksToSeconds( elapsedTicks );
}

uint64 StepTimer::TotalTicks_get() const
{
	return totalTicks;
}

double StepTimer::TotalSeconds_get() const
{
	return TicksToSeconds( totalTicks );
}

uint64 StepTimer::TickCount_get() const
{
	return frameCount;
}

int StepTimer::TicksPerSecond_get() const
{
	return ( int )framesPerSecond;
}

bool StepTimer::IsFixedTimeStep_get() const
{
	return isFixedTimeStep;
}

void StepTimer::IsFixedTimeStep_set( bool value )
{
	isFixedTimeStep = value;
}

uint64 StepTimer::TargetElapsedTicks_get() const
{
	return targetElapsedTicks;
}

void StepTimer::TargetElapsedTicks_set( uint64 value )
{
	targetElapsedTicks = value;
}

double StepTimer::TargetElapsedSeconds_get() const
{
	return TicksToSeconds( targetElapsedTicks );
}

void StepTimer::TargetElapsedSeconds_set( double value )
{
	targetElapsedTicks = SecondsToTicks( value );
}

int StepTimer::TargetElapsedTicksPerSecond_get() const
{
	return ( int )( 1.0 / TargetElapsedSeconds );
}

void StepTimer::TargetElapsedTicksPerSecond_set( int value )
{
	TargetElapsedSeconds = 1.0 / ( double )value;
}

uint64 StepTimer::MaxDeltaTicks_get() const
{
	return nanoMaxDelta;
}

void StepTimer::MaxDeltaTicks_set( uint64 value )
{
	nanoMaxDelta = value;
}

double StepTimer::MaxDeltaSeconds_get() const
{
	return TicksToSeconds( nanoMaxDelta );
}

void StepTimer::MaxDeltaSeconds_set( double value )
{
	nanoMaxDelta = SecondsToTicks( value );
}

uint64 StepTimer::GetNanos()
{
	auto now = clock::now();
	auto dur = now.time_since_epoch().count() - nanoStandard;
	return dur;
}

double StepTimer::TicksToSeconds( uint64 ticks )
{
	return ( double )ticks / ( double )frequency;
}

uint64 StepTimer::SecondsToTicks( double seconds )
{
	return ( uint64 )( seconds * ( double )frequency );
}