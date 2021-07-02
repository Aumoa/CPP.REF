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

public:
	AChessBoard();

	void InitBoard(World* world);
	AChessBoardProxy* CreateProxy();

	Vector3 GetBoardCellPosition(const GridIndex& index) const;
	GridIndex GetGridIndexFromPosition(const Vector3& location) const;

	template<std::derived_from<APiece> T>
	T* SpawnPiece(EChessTeam team, const GridIndex& index)
	{
		World* const world = GetWorld();
		T* piece = world->SpawnActor<T>();
		Internal_SpawnPiece(piece, team, index);
		return piece;
	}

private:
	void Internal_SpawnPiece(APiece* piece, EChessTeam team, const GridIndex& index);
};