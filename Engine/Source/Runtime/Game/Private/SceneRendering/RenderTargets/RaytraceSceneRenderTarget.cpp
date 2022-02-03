// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "SceneRendering/RenderTargets/RaytraceSceneRenderTarget.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "RHI/RHIInterfaces.h"
#include "GameEngine.h"

GENERATE_BODY(SRaytraceSceneRenderTarget);

SRaytraceSceneRenderTarget::SRaytraceSceneRenderTarget() : Super()
{
}

void SRaytraceSceneRenderTarget::ResizeBuffers(const Vector2N& Size)
{
	if (ColorBuffer)
	{
		ColorBuffer->Dispose();
		ColorBuffer = nullptr;
	}

	auto* const RenderSystem = GEngine->GetEngineSubsystem<SGameRenderSystem>();
	if (RenderSystem)
	{
		IRHIDevice* Device = RenderSystem->GetRHIDevice();
		RHITexture2DDesc ColorBufferDesc =
		{
			.Width = (uint32)Size.X,
			.Height = (uint32)Size.Y,
			.DepthOrArraySize = 1,
			.MipLevels = 1,
			.Format = ERHIPixelFormat::R8G8B8A8_UNORM,
			.Usage = ERHIBufferUsage::Default,
			.Flags = ERHIResourceFlags::None,
			.SampleDesc = { 1, 0 },
			.InitialState = ERHIResourceStates::RenderTarget,
			.ClearValue = RHITexture2DClearValue::InitColor(ERHIPixelFormat::R8G8B8A8_UNORM, NamedColors::Transparent)
		};
		ColorBuffer = Device->CreateTexture2D(ColorBufferDesc, nullptr);
	}
}

void SRaytraceSceneRenderTarget::Dispose(bool bDisposing)
{
	if (bDisposing)
	{
		if (ColorBuffer)
		{
			ColorBuffer->Dispose();
			ColorBuffer = nullptr;
		}
	}
}