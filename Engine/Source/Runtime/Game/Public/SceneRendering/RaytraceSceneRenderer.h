// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneRenderer.h"

class SRaytraceSceneRenderer : public SSceneRenderer
{
	GENERATED_BODY(SRaytraceSceneRenderer)

public:
	SRaytraceSceneRenderer();

	virtual void DrawScene(SScene* Scene) override;
};