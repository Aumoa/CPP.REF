// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Threading/CancellationToken.h"
#include "OperationCanceledException.h"

namespace Ayla
{
	ManagedCancellationTokenWrapper CancellationToken::AsManaged()
	{
		CancellationTokenSource::static__CancellationTokenSource();

		if (m_Source)
		{
			auto lock = std::unique_lock(m_Source->m_MarshalLock);
			if (m_Source->m_GCHandle == 0)
			{
				m_Source->m_GCHandle = CancellationTokenSource::coreclr__create_cancellation_token_source();
				std::function<void()> f = std::bind(&CancellationTokenSource::managed_stop_source::CancelManaged, m_Source.get());
				m_Source->m_Callback = new std::stop_callback(m_Source->m_Source.get_token(), f);
			}

			return ManagedCancellationTokenWrapper
			{
				.Handle = m_Source->m_GCHandle,
				.Ptr = (ssize_t)m_Source->m_Callback
			};
		}
		else
		{
			return ManagedCancellationTokenWrapper
			{
				.Handle = 0,
				.Ptr = 0
			};
		}
	}

	[[noreturn]]
	void CancellationToken::ThrowOperationCanceledException()
	{
		throw OperationCanceledException();
	}
}