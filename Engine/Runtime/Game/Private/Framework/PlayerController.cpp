// Copyright 2020 Aumoa.lib. All right reserved.

#include "Framework/PlayerController.h"

#include "Framework/Pawn.h"
#include "Components/PlayerCameraManager.h"
#include "Diagnostics/ScopedCycleCounter.h"

DEFINE_STATS_GROUP(APlayerController);

APlayerController::APlayerController() : Super()
	, cameraManager(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	cameraManager = AddComponent<PlayerCameraManager>();
}

APlayerController::~APlayerController()
{

}

void APlayerController::Tick(Seconds deltaTime)
{
	QUICK_SCOPED_CYCLE_COUNTER(APlayerController, Tick);

	Super::Tick(deltaTime);
}

void APlayerController::OnPossess(APawn* inPawn)
{
	inPawn->ComponentAdded += bind_delegate(Possessed_ComponentAdded);
	cameraManager->UpdateCameraComponent();
}

void APlayerController::OnUnPossess()
{
	GetPawn()->ComponentAdded -= bind_delegate(Possessed_ComponentAdded);
}

PlayerCameraManager* APlayerController::CameraManager_get() const
{
	return cameraManager;
}

void APlayerController::Possessed_ComponentAdded(ActorComponent*)
{
	cameraManager->UpdateCameraComponent();
}