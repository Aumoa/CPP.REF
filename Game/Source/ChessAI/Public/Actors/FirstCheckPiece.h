// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Piece.h"

class CHESSAI_API AFirstCheckPiece : public APiece
{
	GENERATED_BODY(AFirstCheckPiece)

private:
	uint8 _bFirstMove : 1 = true;

public:
	AFirstCheckPiece();

	virtual ActionRecord Move(const GridIndex& index, const ChessBoardBuilt& built) override;

	inline bool IsFirst() const { return _bFirstMove; }
};