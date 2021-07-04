// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ChessPlayerController.h"
#include "LogChess.h"
#include "Level/World.h"
#include "Level/GameLevel.h"
#include "Actors/ChessBoard.h"
#include "Pawns/ChessBoardProxy.h"
#include "Components/InputComponent.h"
#include "Components/IndicatingComponent.h"
#include "Components/CommandComponent.h"

AChessPlayerController::AChessPlayerController() : Super()
{
	_systems.emplace_back(_indicatingComponent = CreateSubobject<IndicatingComponent>());
	_systems.emplace_back(_commandComponent = CreateSubobject<CommandComponent>());
}

void AChessPlayerController::BeginPlay()
{
	Super::BeginPlay();

	World* world = GetWorld();
	Level* level = world->GetLevel();
	GameLevel* gameLevel = dynamic_cast<GameLevel*>(level);
	checkf(gameLevel != nullptr, L"Level is not a GameLevel class.");

	_board = gameLevel->GetPersistentChessBoard();
	AChessBoardProxy* proxy = _board->CreateProxy(EChessTeam::White);
	Possess(proxy);

	proxy->SetActorLocation(Vector3(0, 10.0f, -10.0f));
	proxy->SetActorRotation(Quaternion::LookTo(Vector3(0, -10.0f, 10.0f), Vector3(0, 1.0f, 0)));

	// Setting up chess board on system component.
	for (auto& system : _systems)
	{
		system->SetupBoard(proxy);
	}

	// Post initialize system component.
	_indicatingComponent->ActionRequest.AddObject(_commandComponent, &CommandComponent::DoCommand);

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
		_indicatingComponent->UpdateHoverIndicator(dst);
	});

	inputComponent->MouseEvent.AddRaw([this](EMouseButton button, EMouseButtonEvent event)
	{
		if (button == EMouseButton::Left && event == EMouseButtonEvent::Pressed)
		{
			_indicatingComponent->UpdateSelected();
		}
	});
}