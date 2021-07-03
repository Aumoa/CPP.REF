// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "Components/ActorComponent.h"

class AChessBoard;
class AChessBoardProxy;

class ChessSystemComponent : public ActorComponent
{
public:
	using Super = ActorComponent;

private:
	AChessBoard* _board = nullptr;
	AChessBoardProxy* _boardProxy = nullptr;

public:
	ChessSystemComponent();

	virtual void SetupBoard(AChessBoardProxy* board);

	inline AChessBoard* GetBoard() const { return _board; }
	inline AChessBoardProxy* GetProxy() const { return _boardProxy; }
};