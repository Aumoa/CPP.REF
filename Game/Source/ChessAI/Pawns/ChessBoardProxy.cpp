// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ChessBoardProxy.h"
#include "Camera/CameraComponent.h"

AChessBoardProxy::AChessBoardProxy(AChessBoard* chessBoard) : Super()
	, _board(chessBoard)
{
	CameraComponent* cp = CreateSubobject<CameraComponent>();
	SetRootComponent(cp);
}