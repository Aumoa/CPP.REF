// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Framework/PlayerController.h"

#include "Framework/Pawn.h"
#include "Components/PlayerCameraManager.h"
#include "Diagnostics/ScopedCycleCounter.h"
#include "Components/InputComponent.h"
#include "Input/Keyboard.h"
#include "Windows/WinMouse.h"

DEFINE_STATS_GROUP(APlayerController);

APlayerController::APlayerController() : Super()
	, bAutoUnlocked(false)

	, cameraManager(nullptr)
	, inputComponent(nullptr)
	
	, bShowMouseCursor(true)
	, bAutoUnlockMouseCursor(true)
{
	PrimaryActorTick.bCanEverTick = true;

	cameraManager = AddComponent<GPlayerCameraManager>();
	inputComponent = AddComponent<GInputComponent>();

	keyTracker = inputComponent->KeyTracker;
	mouseTracker = inputComponent->MouseTracker;
}

APlayerController::~APlayerController()
{

}

void APlayerController::Tick(Seconds deltaTime)
{
	Super::Tick(deltaTime);

	UpdateInput();
	UpdateCursorVisibleState();
}

void APlayerController::OnPossess(APawn* inPawn)
{
	inPawn->ComponentAdded += bind_delegate(Possessed_ComponentAdded);
	cameraManager->UpdateCameraComponent();

	inPawn->SetupPlayerInputComponent(inputComponent);
}

void APlayerController::OnUnPossess()
{
	GetPawn()->ComponentAdded -= bind_delegate(Possessed_ComponentAdded);
	cameraManager->UpdateCameraComponent();
}

GPlayerCameraManager* APlayerController::CameraManager_get() const
{
	return cameraManager;
}

GInputComponent* APlayerController::PlayerInputComponent_get() const
{
	return inputComponent;
}

bool APlayerController::IsCursorLocked_get() const
{
	return !bShowMouseCursor && !bAutoUnlocked;
}

void APlayerController::UpdateInput()
{
	if (bAutoUnlockMouseCursor)
	{
		if (keyTracker->IsKeyPressed(EKey::Escape))
		{
			bAutoUnlocked = true;
		}

		if (mouseTracker->IsButtonPressed(EMouseButton::Left) || mouseTracker->IsButtonPressed(EMouseButton::Right))
		{
			bAutoUnlocked = false;
		}
	}
}

void APlayerController::UpdateCursorVisibleState()
{
	bool bCursorVisible = mouseTracker->Last.Mode == EMousePositionMode::Absolute;
	bool bDesiredVisible = !IsCursorLocked;

	if (bCursorVisible != bDesiredVisible)
	{
		EMousePositionMode mode = bDesiredVisible ? EMousePositionMode::Absolute : EMousePositionMode::Relative;
		WinMouse::Get().SetMode(mode);
	}
}

void APlayerController::Possessed_ComponentAdded(GActorComponent*)
{
	cameraManager->UpdateCameraComponent();
}