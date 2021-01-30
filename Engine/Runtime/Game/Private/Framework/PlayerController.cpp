// Copyright 2020 Aumoa.lib. All right reserved.

#include "Framework/PlayerController.h"

#include "Framework/Pawn.h"
#include "Components/PlayerCameraManager.h"
#include "Diagnostics/ScopedCycleCounter.h"
#include "Components/InputComponent.h"

DEFINE_STATS_GROUP(APlayerController);

APlayerController::APlayerController() : Super()
	, cameraManager(nullptr)
	, inputComponent(nullptr)
{
	cameraManager = AddComponent<PlayerCameraManager>();
	inputComponent = AddComponent<InputComponent>();
}

APlayerController::~APlayerController()
{

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

void APlayerController::Possessed_ComponentAdded(ActorComponent*)
{
	cameraManager->UpdateCameraComponent();
}