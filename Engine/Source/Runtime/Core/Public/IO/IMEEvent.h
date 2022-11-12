// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"

enum class EIMEEvent
{
	Char,
	StartComposition,
	EndComposition,
	Composition
};

struct IMEEvent
{
	EIMEEvent Event;

	union
	{
		struct
		{
			char_t ComposedChar;
		} Char;

		struct
		{
			uint8 _unused;
		} StartComposition;

		struct
		{
			uint8 _unused;
		} EndComposition;

		struct
		{
			char_t ComposingChar;
		} Composition;
	};

	IMEEvent(EIMEEvent Event)
		: Event(Event)
		, Char{ 0 }
	{
	}
};