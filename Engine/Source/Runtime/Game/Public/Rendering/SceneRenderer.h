// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneRenderer.gen.h"

class SceneView;
class RHICommandList;

SCLASS()
class GAME_API SceneRenderer : implements Object
{
	GENERATED_BODY()

protected:
	SceneRenderer();
	virtual ~SceneRenderer() noexcept;

public:
	virtual void Render(SceneView* Scene, RHICommandList* CmdList) = 0;
};