// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Controller/ChessPlayerController.h"
#include "LogChess.h"
#include "GameEngine.h"
#include "Level/World.h"
#include "Level/GameLevel.h"
#include "Actors/ChessBoard.h"
#include "Pawns/ChessBoardProxy.h"
#include "Components/IndicatingComponent.h"
#include "Components/CommandComponent.h"
#include "EngineSubsystems/GameInputSystem.h"

AChessPlayerController::AChessPlayerController() : Super()
{
	_systems.emplace_back(_indicatingComponent = NewObject<IndicatingComponent>());
	_systems.emplace_back(_commandComponent = NewObject<CommandComponent>());
}

void AChessPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SWorld* world = GetWorld();
	SLevel* level = world->GetLevel();
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
	_indicatingComponent->ActionRequest.AddSObject(_commandComponent, &CommandComponent::DoCommand);

	SetupPlayerInput();
}

void AChessPlayerController::SetupPlayerInput()
{
	auto* inputSystem = GEngine->GetEngineSubsystem<SGameInputSystem>();
	inputSystem->Keyboard.AddRaw([&](EKey key, EKeyboardEvent keyEvent)
	{
		if (key == EKey::LeftControl)
		{
			if (keyEvent == EKeyboardEvent::Pressed)
			{
				_bLeftCtrl = true;
			}
			else
			{
				_bLeftCtrl = false;
			}
		}

		if (keyEvent == EKeyboardEvent::Pressed)
		{
			if (key == EKey::Z)
			{
				_commandComponent->Undo();
			}
		}
	});

	inputSystem->MouseMove.AddRaw([this](const auto& move)
	{
		Ray toWorldRay = ScreenPointToRay(move.CurX, move.CurY);
		float distance = toWorldRay.Origin[1] / -toWorldRay.Direction[1];
		Vector3 dst = toWorldRay.Origin + toWorldRay.Direction * distance;
		_indicatingComponent->UpdateHoverIndicator(dst);
	});

	inputSystem->Mouse.AddRaw([this](Vector2N location, EMouseButton button, EMouseButtonEvent event)
	{
		if (button == EMouseButton::Left && event == EMouseButtonEvent::Pressed)
		{
			_indicatingComponent->UpdateSelected();
		}
	});
}