// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "ChessAIStructures.h"

class AChessBoard;

struct MovablePointsArray
{
	enum class FigureType
	{
		Move,
		Attack,
	};

	FigureType Type;
	std::vector<GridIndex> Points;
};

struct MovablePointsQuery
{
	std::vector<MovablePointsArray> Results;

	inline MovablePointsArray* BeginFigure(MovablePointsArray::FigureType figureType)
	{
		MovablePointsArray* figure = &Results.emplace_back();
		figure->Type = figureType;
		return figure;
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

	inline size_t GetPointsCount(MovablePointsArray::FigureType figureType) const
	{
		size_t n = 0;
		for (size_t i = 0; i < Results.size(); ++i)
		{
			if (Results[i].Type == figureType)
			{
				n += Results[i].Points.size();
			}
		}
		return n;
	}

	inline const MovablePointsArray* GetHit(const GridIndex& loc) const
	{
		for (auto& figure : Results)
		{
			for (auto& point : figure.Points)
			{
				if (point == loc)
				{
					return &figure;
				}
			}
		}

		return nullptr;
	}
};