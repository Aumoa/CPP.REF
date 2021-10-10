// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

class AChessBoard;
class AChessBoardProxy;

class CHESS_API ChessSystemComponent : public SActorComponent
{
	GENERATED_BODY(ChessSystemComponent)

private:
	AChessBoard* _board = nullptr;
	AChessBoardProxy* _boardProxy = nullptr;

public:
	ChessSystemComponent();

	virtual void SetupBoard(AChessBoardProxy* board);

	inline AChessBoard* GetBoard() const { return _board; }
	inline AChessBoardProxy* GetProxy() const { return _boardProxy; }
};