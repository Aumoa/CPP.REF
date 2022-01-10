// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

enum class EIMEEvent
{
	Char
};

struct IMEEvent
{
	EIMEEvent Event;

	union
	{
		struct
		{
			wchar_t ComposedChar;
		} Char;
	};

	IMEEvent(EIMEEvent Event) : Event(Event)
	{
	}
};