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

	std::wstring ToString() const
	{
		if (!IsValid())
		{
			return L"<Invalid>";
		}
		else
		{
			static wchar_t Horzs[] = L"ABCDEFGH";
			static wchar_t Verts[] = L"12345678";
			wchar_t buff[] = { Horzs[X], Verts[Y], 0 };
			return buff;
		}
	}

	constexpr bool IsValid() const
	{
		return X >= 0 && X < 8
			&& Y >= 0 && Y < 8;
	}

	constexpr bool operator ==(const GridIndex& rhs) const
	{
		return X == rhs.X && Y == rhs.Y;
	}

	constexpr bool operator !=(const GridIndex& rhs) const
	{
		return X != rhs.X || Y != rhs.Y;
	}
};