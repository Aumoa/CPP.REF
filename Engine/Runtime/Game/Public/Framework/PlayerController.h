// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Controller.h"

class PlayerCameraManager;

class GAME_API APlayerController : public AController
{
public:
	using Super = AController;
	using This = APlayerController;

private:
	PlayerCameraManager* cameraManager;

public:
	APlayerController();
	~APlayerController() override;

	void OnPossess(APawn* inPawn) override;
	void OnUnPossess() override;

	vs_property_get(PlayerCameraManager*, CameraManager);
	PlayerCameraManager* CameraManager_get() const;

private:
	void Possessed_ComponentAdded(ActorComponent*);
};