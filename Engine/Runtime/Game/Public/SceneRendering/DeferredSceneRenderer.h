// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "SceneRenderer.h"

class GAME_API DeferredSceneRenderer : public SceneRenderer
{
public:
	using Super = SceneRenderer;
	using This = SceneRenderer;

public:
	DeferredSceneRenderer(IRHIDeviceBundle* deviceBundle);
	~DeferredSceneRenderer() override;

	void BeginRender() override;
	void EndRender() override;
	void PopulateRenderCommands() override;
};