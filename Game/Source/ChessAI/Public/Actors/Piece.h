// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessAIStructures.h"
#include "Queries/ChessQueries.h"

class AChessBoard;
class SStaticMesh;
class SStaticMeshComponent;
class ChessBoardBuilt;

class CHESSAI_API APiece : public AActor
{
	GENERATED_BODY(APiece)

private:
	AChessBoard* _board = nullptr;
	SStaticMeshComponent* _meshComponent = nullptr;
	EChessTeam _team = EChessTeam::Black;
	GridIndex _myIndex;

public:
	APiece();

	virtual void Init(AChessBoard* board, EChessTeam team, const GridIndex& index);
	virtual ActionRecord Move(const GridIndex& index, const ChessBoardBuilt& built);
	virtual void TurnChanged(EChessTeam changedTurn) {};

	virtual bool QueryMovable(MovablePointsQuery& query, const ChessBoardBuilt& built) const = 0;
	virtual bool QueryInteractionWith(MovablePointsQuery& query, APiece* piece, const ChessBoardBuilt& built) const { return true; }

	void SetIndex(const GridIndex& location);
	inline EChessTeam GetTeam() const { return _team; }
	inline GridIndex GetIndex() const { return _myIndex; }
	inline AChessBoard* GetBoard() const { return _board; }

protected:
	virtual SStaticMesh* GetStaticMesh() const = 0;
};