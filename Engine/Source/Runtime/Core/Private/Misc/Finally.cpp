// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/Finally.h"

Finally::Finally(std::function<void()> Action)
	: Action(std::move(Action))
{
}

Finally::~Finally() noexcept
{
	Execute();
}

void Finally::Execute()
{
	if (Action)
	{
		std::exchange(Action, nullptr)();
	}
}