// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "Piece.h"

class AFirstCheckPiece : public APiece
{
public:
	using Super = APiece;

private:
	uint8 _bFirstMove : 1 = true;

public:
	AFirstCheckPiece();

	virtual ActionRecord Move(const GridIndex& index, const ChessBoardBuilt& built) override;

	inline bool IsFirst() const { return _bFirstMove; }
};