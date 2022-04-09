// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline PlatformGeneric
{
	SENUM(PLATFORMGENERIC_API, EIMEEvent, int32,
		Char,,
		StartComposition,,
		EndComposition,,
		Composition,
	);

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
}