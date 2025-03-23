// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

class NSceneRenderer;

class RENDERCORE_API NSceneView
{
public:
	NSceneView();
	~NSceneView() noexcept;

	void Render(NSceneRenderer& Renderer);
};