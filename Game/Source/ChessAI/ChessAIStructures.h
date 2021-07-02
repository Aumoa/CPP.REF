// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "ChessAIEnums.h"

struct GridIndex
{
	int32 X = 0;
	int32 Y = 0;

	constexpr GridIndex()
	{
	}

	constexpr GridIndex(int32 x, int32 y) : X(x), Y(y)
	{
	}

	constexpr bool IsValid() const
	{
		return X >= 0 && X < 8
			&& Y >= 0 && Y < 8;
	}
};