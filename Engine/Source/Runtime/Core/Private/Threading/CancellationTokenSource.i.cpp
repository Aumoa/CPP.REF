// Copyright 2020-2023 Aumoa.lib. All right reserved.

import Core;

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