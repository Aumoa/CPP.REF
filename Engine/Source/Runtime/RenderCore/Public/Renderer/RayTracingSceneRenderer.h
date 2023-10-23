// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "Renderer/SceneRenderer.h"

class RENDERCORE_API NRayTracingSceneRenderer : public NSceneRenderer
{
public:
	NRayTracingSceneRenderer();

	virtual void BeginRender() override;
	virtual void EndRender() override;
};