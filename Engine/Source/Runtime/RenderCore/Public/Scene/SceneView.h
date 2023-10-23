// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

class NSceneRenderer;

class RENDERCORE_API NSceneView
{
public:
	NSceneView();
	~NSceneView() noexcept;

	void Render(NSceneRenderer& Renderer);
};