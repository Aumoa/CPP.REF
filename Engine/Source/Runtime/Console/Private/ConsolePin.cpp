// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "ConsolePin.h"
#include "InternalConsole.h"

using namespace ::libty;

ConsolePin ConsolePin::Reserve(int32 height)
{
	if (height < 1)
	{
		throw ArgumentException("height");
	}

	ConsolePin pin;
	pin._impl = std::make_shared<Impl>();

	LogModule::Get()->EnqueueLogAction([impl = pin._impl, height]() mutable
	{
		impl->_y = InternalConsole::GetCursorPos().second;
		impl->_height = height;

		for (int32 i = 0; i < height; ++i)
		{
			std::cout << std::endl;
		}
	});

	return pin;
}