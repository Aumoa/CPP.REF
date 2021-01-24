// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Diagnostics/ScopedCycleCounter.h"

class APlayerController;
class PlayerCameraManager;
class ShaderCameraConstantVector;
class PrimitiveSceneProxy;

GAME_API DECLARE_STATS_GROUP(SceneVisibility);

class GAME_API SceneVisibility : virtual public Object
{
public:
	using Super = Object;
	using This = SceneVisibility;

private:
	APlayerController* playerController;
	PlayerCameraManager* cameraManager;
	TRefPtr<ShaderCameraConstantVector> cameraConstants;

	std::vector<bool> visibilities;

public:
	SceneVisibility(APlayerController* inPlayerController);
	~SceneVisibility() override;

	void CalcVisibility(const std::vector<PrimitiveSceneProxy*>& sceneProxies);
	const std::vector<bool>& GetVisibilities() const;
	ShaderCameraConstantVector* GetConstantVector() const;

	vs_property_get(bool, IsValid);
	bool IsValid_get() const;
};