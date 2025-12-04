// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "OperationCanceledException.h"

namespace Ayla
{
	class CORE_API TaskCanceledException : public OperationCanceledException
	{
	public:
		TaskCanceledException(std::stop_token stoppingToken = {}, std::source_location src = std::source_location::current())
			: OperationCanceledException(TEXT("Task was canceled."), stoppingToken, nullptr, src)
		{
		}
	};
}