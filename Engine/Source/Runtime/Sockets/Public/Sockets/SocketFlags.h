// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Sockets
{
	SENUM(SOCKETS_API, ESocketFlags, int32,
		None				, 0,
		OutOfBand			, 1,
		Peek				, OutOfBand << 1,
		DontRoute			, Peek << 1,
		WaitAll				, DontRoute << 1
	);
}