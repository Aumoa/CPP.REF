// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "SceneRenderer.h"

class GAME_API ToneMappingSceneRenderer : public SceneRenderer
{
public:
	using Super = SceneRenderer;

public:
	ToneMappingSceneRenderer(IRHIScene* scene);
	~ToneMappingSceneRenderer() override;

	void RenderScene(IRHICommandList* immediateCommandList) override;
};