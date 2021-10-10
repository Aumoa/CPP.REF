// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

class AChessBoard;
class ChessSystemComponent;
class IndicatingComponent;
class CommandComponent;

class CHESS_API AChessPlayerController : public APlayerController
{
	GENERATED_BODY(AChessPlayerController)

private:
	AChessBoard* _board = nullptr;	
	std::vector<ChessSystemComponent*> _systems;
	IndicatingComponent* _indicatingComponent = nullptr;
	CommandComponent* _commandComponent = nullptr;

	uint8 _bLeftCtrl : 1 = false;

public:
	AChessPlayerController();

	virtual void BeginPlay() override;

private:
	void SetupPlayerInput();
};