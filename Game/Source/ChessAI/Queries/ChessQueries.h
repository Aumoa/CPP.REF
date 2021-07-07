// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "ChessAIStructures.h"

class AChessBoard;
class APiece;
struct MovablePointsQuery;

struct MovablePointsArray
{
	enum class FigureType
	{
		Move,
		Attack,
	};

	FigureType Type;
	std::vector<GridIndex> Points;

	bool CheckAndEmplace(const APiece* piece, const GridIndex& location);
};

struct MovablePointsArrayPointer
{
	MovablePointsQuery* Root = nullptr;
	size_t MyIndex = -1;

	MovablePointsArray* operator ->();
};

struct MovablePointsQuery
{
	const APiece* OwnerActor = nullptr;
	std::vector<MovablePointsArray> Results;

	MovablePointsArrayPointer BeginFigure(MovablePointsArray::FigureType figureType);
	size_t GetPointsCount() const;
	size_t GetPointsCount(MovablePointsArray::FigureType figureType) const;

	const MovablePointsArray* GetHit(const GridIndex& loc) const;
};

class ActionRecord
{
private:
	APiece* _actor = nullptr;
	std::function<void()> _undoRecord;
	uint8 _bValid : 1;
	APiece* _actorTaked = nullptr;

public:
	ActionRecord(bool bValid = false);
	ActionRecord(APiece* actor, std::function<void()> undoRecord);

	inline APiece* GetActor() const { return _actor; }
	inline APiece* GetTakeActor() const { return _actorTaked; }
	void Undo() const;
	void TakeActor(APiece* actor);

	bool IsValid() const;
	inline operator bool() const { return IsValid(); }
};