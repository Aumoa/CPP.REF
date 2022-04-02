// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Core::inline Threading::inline Tasks
{
	enum class ETaskStatus
	{
		Created,
		WaitingToRun,
		Running,
		RanToCompletion,
		Faulted,
		Canceled,
	};
}