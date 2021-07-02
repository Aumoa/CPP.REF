// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ChessPlayerController.h"
#include "LogChess.h"
#include "Level/World.h"
#include "Level/GameLevel.h"
#include "Actors/ChessBoard.h"
#include "Actors/GridIndicator.h"
#include "Pawns/ChessBoardProxy.h"
#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"

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
		_board = gameLevel->GetPersistentChessBoard();
		AChessBoardProxy* proxy = _board->CreateProxy();
		Possess(proxy);

		proxy->SetActorLocation(Vector3(0, 10.0f, -10.0f));
		proxy->SetActorRotation(Quaternion::LookTo(Vector3(0, -10.0f, 10.0f), Vector3(0, 1.0f, 0)));

		AGridIndicator* indicator = world->SpawnActor<AGridIndicator>();
		_indicator = dynamic_cast<PrimitiveComponent*>(indicator->GetRootComponent());
		_indicator->AttachToComponent(_board->GetRootComponent());
		_indicator->SetScale(Vector3(1, 0.05f, 1));
		indicator->SetIndicatorColor(NamedColors::Gray);
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

	inputComponent->MouseMoveEvent.AddRaw([this](int32 x, int32 y, int32 dx, int32 dy)
	{
		Ray toWorldRay = ScreenPointToRay(x, y);
		float distance = toWorldRay.Origin[1] / -toWorldRay.Direction[1];
		Vector3 dst = toWorldRay.Origin + toWorldRay.Direction * distance;

		GridIndex gridIndex = _board->GetGridIndexFromPosition(dst);
		if (gridIndex.IsValid())
		{
			Vector3 amendedLocation = _board->GetBoardCellPosition(gridIndex);
			_indicator->SetLocation(amendedLocation);
			_indicator->SetHiddenInGame(false);
		}
		else
		{
			_indicator->SetHiddenInGame(true);
		}
	});
}