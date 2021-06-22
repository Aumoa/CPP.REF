// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "ChessAIStructures.h"

class AChessBoardProxy;

class AChessBoard : public AActor
{
public:
	using Super = AActor;

private:
	Degrees _rotation;

public:
	AChessBoard();

	void InitBoard(World* world);
	AChessBoardProxy* CreateProxy();

	Vector3 GetBoardCellPosition(const GridIndex& index) const;
};