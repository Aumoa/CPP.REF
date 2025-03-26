// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

namespace Ayla
{
	enum class TaskStatus
	{
		Created,
		Running,
		RanToCompletion,
		Faulted,
		Canceled,
	};
}