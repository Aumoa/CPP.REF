// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Threading/CancellationTokenSource.h"
#include "InvalidOperationException.h"
#include "Threading/Tasks/Task.h"

namespace Ayla
{
	void CancellationTokenSource::CancelAfter(const TimeSpan& delay)
	{
		if (m_Source == nullptr)
		{
			ThrowInvalidOperationException();
		}

		std::ignore = Task<>::Delay(delay, GetToken()).ContinueWith([source = m_Source](Task<>) mutable
		{
			source->request_stop();
		});
	}

	[[noreturn]]
	void CancellationTokenSource::ThrowInvalidOperationException()
	{
		throw InvalidOperationException();
	}
}