// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

enum class EIMEEvent
{
	Char,
	StartComposition,
	EndComposition,
	Composition,
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

		struct
		{
			uint8 __Unused;
		} StartComposition;

		struct
		{
			uint8 __Unused;
		} EndComposition;

		struct
		{
			wchar_t ComposingChar;
		} Composition;
	};

	IMEEvent(EIMEEvent Event) : Event(Event)
	{
	}
};