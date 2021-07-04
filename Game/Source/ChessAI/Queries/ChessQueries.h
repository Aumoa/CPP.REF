// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "ChessAIStructures.h"

class AChessBoard;

struct MovablePointsArray
{
	std::vector<GridIndex> Points;
};

struct MovablePointsQuery
{
	enum class QueryType
	{
		Move,
		Attack,
	};

	QueryType Type;
	std::vector<MovablePointsArray> Results;

	inline MovablePointsArray* BeginFigure()
	{
		return &Results.emplace_back();
	}

	inline size_t GetPointsCount() const
	{
		size_t n = 0;
		for (size_t i = 0; i < Results.size(); ++i)
		{
			n += Results[i].Points.size();
		}
		return n;
	}
};