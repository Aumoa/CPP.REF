// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"
#include <stop_token>

namespace Ayla
{
	class CORE_API OperationCanceledException : public Exception
	{
	private:
		const std::stop_token m_StoppingToken;

	public:
		OperationCanceledException(std::stop_token stoppingToken = {}, std::exception_ptr innerException = nullptr, std::source_location src = std::source_location::current())
			: OperationCanceledException(TEXT("Operation was aborted."), stoppingToken, innerException, src)
		{
		}

		OperationCanceledException(String message, std::stop_token stoppingToken = {}, std::exception_ptr innerException = nullptr, std::source_location src = std::source_location::current())
			: Exception(message, innerException, src)
			, m_StoppingToken(std::move(stoppingToken))
		{
		}

		const std::stop_token& GetStoppingToken() const noexcept { return m_StoppingToken; }
	};
}