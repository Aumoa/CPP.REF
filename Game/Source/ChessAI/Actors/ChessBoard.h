// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "Level/World.h"
#include "ChessAIStructures.h"

class AChessBoardProxy;
class APiece;

class AChessBoard : public AActor
{
public:
	using Super = AActor;

private:
	Degrees _rotation;
	APiece* _pieces[8][8] = {};
	EChessTeam _turn = EChessTeam::White;

public:
	AChessBoard();

	void InitBoard(World* world);
	AChessBoardProxy* CreateProxy(EChessTeam team);

	Vector3 GetBoardCellPosition(const GridIndex& index) const;
	GridIndex GetGridIndexFromPosition(const Vector3& location) const;
	bool HasPiece(const GridIndex& index) const;
	APiece* GetPiece(const GridIndex& index) const;

	template<std::derived_from<APiece> T>
	T* SpawnPiece(EChessTeam team, const GridIndex& index)
	{
		World* const world = GetWorld();
		T* piece = world->SpawnActor<T>();
		Internal_SpawnPiece(piece, team, index);
		return piece;
	}

	bool MovePiece(const GridIndex& from, const GridIndex& to);
	inline EChessTeam GetTurn() const { return _turn; }

private:
	void Internal_SpawnPiece(APiece* piece, EChessTeam team, const GridIndex& index);
	bool Internal_MoveTurn();
};