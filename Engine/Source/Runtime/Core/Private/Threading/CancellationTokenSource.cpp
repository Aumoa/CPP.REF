// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Threading/CancellationTokenSource.h"
#include "System/InvalidOperationException.h"
#include "Threading/Tasks/Task.h"

void CancellationTokenSource::CancelAfter(const TimeSpan& delay)
{
	if (hasValue == false)
	{
		ThrowInvalidOperationException();
	}

	std::ignore = Task<>::Delay(delay, GetToken()).ContinueWith([source = get_source()](Task<>) mutable
	{
		source.request_stop();
	});
}

[[noreturn]]
void CancellationTokenSource::ThrowInvalidOperationException()
{
	throw InvalidOperationException();
}