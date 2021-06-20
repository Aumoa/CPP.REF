// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ChessPlayerController.h"
#include "LogChess.h"
#include "Level/World.h"
#include "Level/GameLevel.h"
#include "Actors/ChessBoard.h"
#include "Pawns/ChessBoardProxy.h"
#include "Components/InputComponent.h"

AChessPlayerController::AChessPlayerController() : Super()
{
}

void AChessPlayerController::BeginPlay()
{
	Super::BeginPlay();

	World* world = GetWorld();
	Level* level = world->GetLevel();
	GameLevel* gameLevel = dynamic_cast<GameLevel*>(level);
	if (gameLevel != nullptr)
	{
		AChessBoard* board = gameLevel->GetPersistentChessBoard();
		AChessBoardProxy* proxy = board->CreateProxy();
		Possess(proxy);

		proxy->SetActorLocation(Vector3(0, 0, -10));
	}

	SetupPlayerInput(GetInputComponent());
}

void AChessPlayerController::SetupPlayerInput(InputComponent* inputComponent)
{
	inputComponent->KeyboardEvent.AddRaw([](EKey key, EKeyboardEvent keyEvent)
	{
		if (key == EKey::Escape && keyEvent == EKeyboardEvent::Pressed)
		{
			LogSystem::Log(LogChess, ELogVerbosity::Verbose, L"Escape key pressed.");
		}
	});
}