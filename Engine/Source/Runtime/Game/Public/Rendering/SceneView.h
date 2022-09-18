// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/MinimalViewInfo.h"

class RenderScene;

class GAME_API SceneView
{
private:
	RenderScene* Scene;
	MinimalViewInfo CameraView;

public:
	SceneView(RenderScene* InScene);

	void SetupView(const MinimalViewInfo& InView);
};