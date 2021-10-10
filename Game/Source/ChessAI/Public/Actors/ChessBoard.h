// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Level/World.h"
#include "ChessAIStructures.h"
#include "Queries/ChessQueries.h"
#include "Queries/ChessBoardBuilt.h"

class AChessBoardProxy;
class APiece;

class CHESSAI_API AChessBoard : public AActor
{
	GENERATED_BODY(AChessBoard)
	friend class ChessBoardBuilt;

private:
	Degrees _rotation;
	APiece* _pieces[8][8] = {};
	EChessTeam _turn = EChessTeam::White;
	ChessBoardBuilt _built;

public:
	AChessBoard();

	void InitBoard(SWorld* world);
	AChessBoardProxy* CreateProxy(EChessTeam team);

	Vector3 GetBoardCellPosition(const GridIndex& index) const;
	GridIndex GetGridIndexFromPosition(const Vector3& location) const;
	inline const ChessBoardBuilt& GetBoardBuilt() const { return _built; }

	template<std::derived_from<APiece> T>
	T* SpawnPiece(EChessTeam team, const GridIndex& index)
	{
		SWorld* const world = GetWorld();
		T* piece = world->SpawnActor<T>();
		Internal_SpawnPiece(piece, team, index);
		return piece;
	}

	ActionRecord MovePiece(const GridIndex& from, const GridIndex& to);
	inline EChessTeam GetTurn() const { return _turn; }
	void SimulateMoveQuery(MovablePointsQuery& query) const;
	void RestorePiece(APiece* piece, const GridIndex& index);

private:
	void Internal_SpawnPiece(APiece* piece, EChessTeam team, const GridIndex& index);
	bool Internal_MoveTurn();
};