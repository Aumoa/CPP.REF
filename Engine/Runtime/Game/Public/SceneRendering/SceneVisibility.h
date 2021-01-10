// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class APlayerController;
class PlayerCameraManager;
class ShaderCameraConstantVector;
class SceneRenderer;

class GAME_API SceneVisibility : virtual public Object
{
public:
	using Super = Object;
	using This = SceneVisibility;

private:
	APlayerController* playerController;
	PlayerCameraManager* cameraManager;
	TRefPtr<ShaderCameraConstantVector> cameraConstants;

public:
	SceneVisibility(APlayerController* inPlayerController);
	~SceneVisibility() override;

	void CalcVisibility(SceneRenderer* renderer);

	vs_property_get(bool, IsValid);
	bool IsValid_get() const;
};