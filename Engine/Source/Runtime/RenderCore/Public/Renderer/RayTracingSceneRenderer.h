// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "Renderer/SceneRenderer.h"

class NRHICommandSet;
class NRHIGameShader;

class RENDERCORE_API NRayTracingSceneRenderer : public NSceneRenderer
{
	using Super = NSceneRenderer;

private:
	std::shared_ptr<NRHIViewport> CurrentViewport;
	std::shared_ptr<NRHICommandSet> CommandSet;
	std::shared_ptr<NRHIGameShader> GameShader;

public:
	NRayTracingSceneRenderer();
	virtual ~NRayTracingSceneRenderer() noexcept override;

	virtual void BeginRender() override;
	virtual void EndRender() override;

	virtual void SetViewport(std::shared_ptr<NRHIViewport> InViewport) override;

private:
	void UnbindViewport();
};