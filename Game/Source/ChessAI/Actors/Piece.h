// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "ChessAIStructures.h"
#include "Queries/ChessQueries.h"

class AChessBoard;
class StaticMesh;
class StaticMeshComponent;

class APiece : public AActor
{
public:
	using Super = AActor;

private:
	AChessBoard* _board = nullptr;
	StaticMeshComponent* _meshComponent = nullptr;
	EChessTeam _team = EChessTeam::Black;
	GridIndex _myIndex;

public:
	APiece();

	virtual void Init(AChessBoard* board, EChessTeam team, const GridIndex& index);
	virtual ActionRecord Move(const GridIndex& index);

	virtual bool QueryMovable(MovablePointsQuery& query) const = 0;
	virtual bool QueryInteractionWith(MovablePointsQuery& query, APiece* piece) const { return true; }

	inline EChessTeam GetTeam() const { return _team; }
	inline GridIndex GetIndex() const { return _myIndex; }
	inline AChessBoard* GetBoard() const { return _board; }

protected:
	virtual StaticMesh* GetStaticMesh() const = 0;
};