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
		AChessBoard* board = gameLevel->GetPersistentChessBoard();
		AChessBoardProxy* proxy = board->CreateProxy();
		Possess(proxy);

		proxy->SetActorLocation(Vector3(0, 10.0f, -10.0f));
		proxy->SetActorRotation(Quaternion::LookTo(Vector3(0, -10.0f, 10.0f), Vector3(0, 1.0f, 0)));

		AActor* indicator = world->SpawnActor<AGridIndicator>();
		_indicator = dynamic_cast<PrimitiveComponent*>(indicator->GetRootComponent());
		_indicator->AttachToComponent(board->GetRootComponent());
		_indicator->SetScale(Vector3(1, 0.05f, 1));
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

		SceneComponent* parentComponent = _indicator->GetAttachParent();
		Transform relativeTransform = parentComponent->GetRelativeTransform();
		Transform inversedTransform = relativeTransform.GetInverse();
		dst = inversedTransform.TransformPoint(dst);

		int32 dstx = (int32)(dst.X() - 0.5f);
		int32 dstz = (int32)(dst.Z() - 0.5f);

		if (dstx <= 0 && dstx > -8 &&
			dstz <= 0 && dstz > -8)
		{
			dst.X() = (float)dstx;
			dst.Z() = (float)dstz;

			_indicator->SetLocation(dst);
			_indicator->SetHiddenInGame(false);
		}
		else
		{
			_indicator->SetHiddenInGame(true);
		}
	});
}