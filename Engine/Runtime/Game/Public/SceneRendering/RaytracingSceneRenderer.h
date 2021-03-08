// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "SceneRenderer.h"

class GAME_API RaytracingSceneRenderer : public SceneRenderer
{
public:
	using Super = SceneRenderer;

public:
	RaytracingSceneRenderer(IRHIScene* scene);
	~RaytracingSceneRenderer() override;

	void RenderScene(IRHICommandList* immediateCommandList) override;
};