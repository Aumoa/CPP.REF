// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DeferredGameViewport.h"

#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXHDRTarget.h"
#include "DirectX/DirectXCompatibleRenderTarget.h"
#include "Logging/LogMacros.h"
#include "Diagnostics/ScopedCycleCounter.h"
#include "SceneRendering/DeferredSceneRenderer.h"
#include "RHI/IRHIScene.h"

DEFINE_STATS_GROUP(DeferredGameViewport);

DeferredGameViewport::DeferredGameViewport(DirectXDeviceBundle* deviceBundle) : Super()
{
	DirectXNew(hdrTarget, DirectXHDRTarget, deviceBundle);
}

DeferredGameViewport::~DeferredGameViewport()
{

}

void DeferredGameViewport::RenderScene(IRHICommandList* inCommandList, IRHIScene* inScene)
{
	DeferredSceneRenderer renderer(inScene);

	{
		QUICK_SCOPED_CYCLE_COUNTER(DeferredGameViewport, CalcVisibility);
		inScene->CalcVisibility();
	}
}

IRHIResource* DeferredGameViewport::GetRenderTarget() const
{
	return nullptr;
}

void DeferredGameViewport::SetViewportResolution_Internal(int32 x, int32 y)
{
	Super::SetViewportResolution_Internal(x, y);

	if (x == 0 || y == 0) {
		return;
	}

	hdrTarget->ResizeBuffers(x, y);
}

void DeferredGameViewport::LightRender(IRHICommandList* inCommandList, IRHIScene* inScene)
{

}

void DeferredGameViewport::TonemapRender(IRHICommandList* inCommandList)
{

}