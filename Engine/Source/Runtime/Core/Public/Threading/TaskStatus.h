// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

enum class ETaskStatus
{
	Created,
	WaitingToRun,
	Running,
	RanToCompletion,
	Faulted,
	Canceled,
};