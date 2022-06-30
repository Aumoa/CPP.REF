// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Game
{
	SENUM(GAME_API, ETickingGroup, int32,
		PrePhysics, 0,
		DuringPhysics, 1,
		PostPhysics, 2,
		PostUpdateWork, 3,
		NumGroups,
	);
}