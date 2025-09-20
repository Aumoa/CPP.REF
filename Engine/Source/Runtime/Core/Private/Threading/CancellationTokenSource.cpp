// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Threading/CancellationTokenSource.h"
#include "InvalidOperationException.h"
#include "Threading/Tasks/Task.h"
#include "ScriptingBackend/ScriptingBackend.h"

namespace Ayla
{
	ssize_t(*CancellationTokenSource::coreclr__create_cancellation_token_source)();
	void(*CancellationTokenSource::coreclr__free_cancellation_token_source)(ssize_t);
	void(*CancellationTokenSource::coreclr__cancel)(ssize_t);

	CancellationTokenSource::managed_stop_source::~managed_stop_source()
	{
		if (m_GCHandle != 0)
		{
			coreclr__free_cancellation_token_source(m_GCHandle);
			delete m_Callback;
		}
	}

	void CancellationTokenSource::managed_stop_source::Cancel()
	{
		m_Source.request_stop();
	}

	void CancellationTokenSource::managed_stop_source::CancelManaged()
	{
		coreclr__cancel(m_GCHandle);
	}

	void CancellationTokenSource::static__CancellationTokenSource()
	{
		static int32 _ = []()
		{
			auto& sb = ScriptingBackend::Get();
			coreclr__create_cancellation_token_source = (ssize_t(*)())sb.GetFunctionPointer("Core.Script", "Ayla.CancellationTokenMarshal", "CreateCancellationTokenSource");
			coreclr__free_cancellation_token_source = (void(*)(ssize_t))sb.GetFunctionPointer("Core.Script", "Ayla.CancellationTokenMarshal", "FreeCancellationTokenSource");
			coreclr__cancel = (void(*)(ssize_t))sb.GetFunctionPointer("Core.Script", "Ayla.CancellationTokenMarshal", "Cancel");
			return 0;
		}();
	}

	CancellationToken CancellationTokenSource::GetToken() const
	{
		if (m_Source == nullptr)
		{
			ThrowInvalidOperationException();
		}

		CancellationToken token;
		token.m_Source = m_Source;
		token.m_Token = m_Source->m_Source.get_token();
		return token;
	}

	void CancellationTokenSource::Cancel()
	{
		if (m_Source == nullptr)
		{
			ThrowInvalidOperationException();
		}

		m_Source->Cancel();
	}

	void CancellationTokenSource::CancelAfter(const TimeSpan& delay)
	{
		if (m_Source == nullptr)
		{
			ThrowInvalidOperationException();
		}

		std::ignore = Task<>::Delay(delay, GetToken()).ContinueWith([source = m_Source](Task<>) mutable
		{
			source->m_Source.request_stop();
		});
	}

	[[noreturn]]
	void CancellationTokenSource::ThrowInvalidOperationException()
	{
		throw InvalidOperationException();
	}
}