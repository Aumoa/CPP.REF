// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ChessAIStructures.h"

class AChessBoard;
class APiece;
class ChessBoardBuilt;
struct MovablePointsQuery;

struct CHESSAI_API MovablePointsArray
{
	enum class FigureType
	{
		Move,
		Attack,
		Special,
	};

	FigureType Type;
	std::vector<GridIndex> Points;
	std::vector<APiece*> Targets;

	bool CheckAndEmplace(const APiece* piece, const GridIndex& location, const ChessBoardBuilt& built, APiece* target = nullptr);
};

struct CHESSAI_API MovablePointsArrayPointer
{
	MovablePointsQuery* Root = nullptr;
	size_t MyIndex = -1;

	MovablePointsArray* operator ->();
};

struct CHESSAI_API MovablePointsQuery
{
	const APiece* OwnerActor = nullptr;
	std::vector<MovablePointsArray> Results;

	MovablePointsArrayPointer BeginFigure(MovablePointsArray::FigureType figureType);
	size_t GetPointsCount() const;
	size_t GetPointsCount(MovablePointsArray::FigureType figureType) const;

	std::pair<const MovablePointsArray*, size_t> GetHit(const GridIndex& loc) const;
};

class CHESSAI_API ActionRecord
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