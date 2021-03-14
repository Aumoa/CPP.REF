// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Controller.h"

class GPlayerCameraManager;
class GInputComponent;
class Keyboard;
class KeyStateTracker;
class MouseStateTracker;

class GAME_API APlayerController : public AController
{
public:
	using Super = AController;
	using This = APlayerController;

private:
	bool bAutoUnlocked : 1;

	GPlayerCameraManager* cameraManager;
	GInputComponent* inputComponent;

	KeyStateTracker* keyTracker;
	MouseStateTracker* mouseTracker;

public:
	APlayerController();
	~APlayerController() override;

	void Tick(Seconds deltaTime) override;

	void OnPossess(APawn* inPawn) override;
	void OnUnPossess() override;

	vs_property_get(GPlayerCameraManager*, CameraManager);
	vs_property_get(GInputComponent*, PlayerInputComponent);
	vs_property_get(bool, IsCursorLocked);

	bool bShowMouseCursor : 1;
	bool bAutoUnlockMouseCursor : 1;

private:
	void UpdateInput();
	void UpdateCursorVisibleState();

	void Possessed_ComponentAdded(GActorComponent*);
};