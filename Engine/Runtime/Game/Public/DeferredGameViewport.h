// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "GameViewport.h"

class GAME_API DeferredGameViewport : public GameViewport
{
public:
	using Super = GameViewport;
	using This = DeferredGameViewport;

private:

public:
	DeferredGameViewport();
	~DeferredGameViewport() override;

	void RenderScene(IRHICommandList* inCommandList, Scene* inScene) override;

protected:
	void SetViewportResolution_Internal(int32 x, int32 y) override;
};