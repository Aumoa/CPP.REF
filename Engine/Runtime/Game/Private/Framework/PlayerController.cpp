// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Framework/PlayerController.h"

#include "Framework/Pawn.h"
#include "Components/PlayerCameraManager.h"
#include "Diagnostics/ScopedCycleCounter.h"
#include "Components/InputComponent.h"
#include "PlatformMisc/PlatformInput.h"

DEFINE_STATS_GROUP(APlayerController);

APlayerController::APlayerController() : Super()
	, bAutoUnlocked(false)

	, cameraManager(nullptr)
	, inputComponent(nullptr)
	
	, bShowMouseCursor(true)
	, bAutoUnlockMouseCursor(true)
{
	PrimaryActorTick.bCanEverTick = true;

	cameraManager = AddComponent<PlayerCameraManager>();
	inputComponent = AddComponent<InputComponent>();

	inputComponent->GetKeyActionBinder(EKey::ESC, EKeyEvent::Pressed).AddRaw(this, &APlayerController::AutoUnlockMouseCursor);
	inputComponent->GetKeyActionBinder(EKey::LeftButton, EKeyEvent::Pressed).AddRaw(this, &APlayerController::AutoUnlockMouseCursor);
	inputComponent->GetKeyActionBinder(EKey::RightButton, EKeyEvent::Pressed).AddRaw(this, &APlayerController::AutoUnlockMouseCursor);
}

APlayerController::~APlayerController()
{

}

void APlayerController::Tick(Seconds deltaTime)
{
	Super::Tick(deltaTime);

	UpdateCursorVisibleState();
}

void APlayerController::OnPossess(APawn* inPawn)
{
	inPawn->ComponentAdded += bind_delegate(Possessed_ComponentAdded);
	cameraManager->UpdateCameraComponent();

	// Binding input override component.
	possessedInputComponent = inPawn->AddComponent<InputComponent>();
	inputComponent->SetOverrideComponent(possessedInputComponent);

	inPawn->SetupPlayerInputComponent(possessedInputComponent);
}

void APlayerController::OnUnPossess()
{
	// Unbinding input override component.
	if (GetPawn()->RemoveComponent<InputComponent>())
	{
		inputComponent->SetOverrideComponent(nullptr);
	}

	GetPawn()->ComponentAdded -= bind_delegate(Possessed_ComponentAdded);
	cameraManager->UpdateCameraComponent();
}

PlayerCameraManager* APlayerController::CameraManager_get() const
{
	return cameraManager;
}

InputComponent* APlayerController::PlayerInputComponent_get() const
{
	return inputComponent;
}

bool APlayerController::IsCursorLocked_get() const
{
	return !bShowMouseCursor && !bAutoUnlocked;
}

void APlayerController::UpdateCursorVisibleState()
{
	bool bCursorVisible = PlatformInput::GetCursorState().IsCursorVisible();
	bool bDesiredVisible = !IsCursorLocked;

	if (bCursorVisible != bDesiredVisible)
	{
		PlatformInput::SetCursorVisible(bDesiredVisible);
	}
}

void APlayerController::Possessed_ComponentAdded(ActorComponent*)
{
	cameraManager->UpdateCameraComponent();
}

void APlayerController::AutoUnlockMouseCursor(EKey inKey, EKeyEvent inKeyEvent)
{
	if (bAutoUnlockMouseCursor)
	{
		switch (inKey)
		{
		case EKey::ESC:
			bAutoUnlocked = true;
			break;
		case EKey::LeftButton:
		case EKey::RightButton:
			bAutoUnlocked = false;
			break;
		}
	}
}