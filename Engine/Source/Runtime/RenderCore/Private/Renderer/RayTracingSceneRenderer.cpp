// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Renderer/RayTracingSceneRenderer.h"
#include "RHI/RHIViewport.h"
#include "RHI/RHICommandSet.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"
#include "RHI/RHICommandQueue.h"

NRayTracingSceneRenderer::NRayTracingSceneRenderer()
{
	auto& Graphics = NRHIGlobal::GetDynamicRHI();
	CommandSet = Graphics.CreateCommandSet();
	GameShader = Graphics.CreateGameShader();
}

NRayTracingSceneRenderer::~NRayTracingSceneRenderer() noexcept
{
}

void NRayTracingSceneRenderer::BeginRender()
{
	CommandSet->BeginFrame();
}

void NRayTracingSceneRenderer::EndRender()
{
	UnbindViewport();
	CommandSet->EndFrame();

	auto& Graphics = NRHIGlobal::GetDynamicRHI();
	Graphics.GetPrimaryQueue()->ExecuteCommandSets(std::vector<NRHICommandSet*>{ CommandSet.get() });
}

void NRayTracingSceneRenderer::SetViewport(std::shared_ptr<NRHIViewport> InViewport)
{
	UnbindViewport();
	Super::SetViewport(InViewport);
	CommandSet->BeginRender(*InViewport, true);
	CurrentViewport = InViewport;
}

void NRayTracingSceneRenderer::UnbindViewport()
{
	if (CurrentViewport)
	{
		CommandSet->EndRender(*CurrentViewport);
		CurrentViewport = nullptr;
	}
}