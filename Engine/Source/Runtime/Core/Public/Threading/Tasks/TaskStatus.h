// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Core
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