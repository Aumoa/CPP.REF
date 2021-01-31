// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Controller.h"

#include "Diagnostics/ScopedCycleCounter.h"
#include "Key.h"

class PlayerCameraManager;
class InputComponent;

GAME_API DECLARE_STATS_GROUP(APlayerController);

class GAME_API APlayerController : public AController
{
public:
	using Super = AController;
	using This = APlayerController;

private:
	bool bAutoUnlocked : 1;

	PlayerCameraManager* cameraManager;
	InputComponent* inputComponent;
	InputComponent* possessedInputComponent;

public:
	APlayerController();
	~APlayerController() override;

	void Tick(Seconds deltaTime) override;

	void OnPossess(APawn* inPawn) override;
	void OnUnPossess() override;

	vs_property_get(PlayerCameraManager*, CameraManager);
	PlayerCameraManager* CameraManager_get() const;
	vs_property_get(InputComponent*, PlayerInputComponent);
	InputComponent* PlayerInputComponent_get() const;
	vs_property_get(bool, IsCursorLocked);
	bool IsCursorLocked_get() const;

	bool bShowMouseCursor : 1;
	bool bAutoUnlockMouseCursor : 1;

private:
	void UpdateCursorVisibleState();

	void Possessed_ComponentAdded(ActorComponent*);
	void AutoUnlockMouseCursor(EKey inKey, EKeyEvent inKeyEvent);
};