// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Console
{
	SENUM(CONSOLE_API, EConsoleModifiers, int32,
		None, 0,
		Alt, 0x1,
		Shift, 0x2,
		Control, 0x4
	);

	SENUM_MAKE_FLAGS(EConsoleModifiers);
}