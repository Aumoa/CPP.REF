// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ChessAIStructures.h"

class APiece;
class AChessBoard;

class CHESSAI_API ChessBoardBuilt
{
	struct Mark
	{
		APiece* PlacedActor;
		uint8 bMarkAttackWhite : 1;
		uint8 bMarkAttackBlack : 1;
	};

	const AChessBoard* _board = nullptr;
	Mark _marks[8][8] = {};

public:
	ChessBoardBuilt(const AChessBoard* board);

	void Init();
	void Build();
	void SimulateMoveAndBuild(const GridIndex& from, const GridIndex& to);

	bool HasPiece(const GridIndex& index) const;
	APiece* GetPiece(const GridIndex& index) const;

	bool IsChecked(GridIndex& index) const;
};