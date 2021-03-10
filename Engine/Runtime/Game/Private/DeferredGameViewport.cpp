// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DeferredGameViewport.h"

#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXHDRTarget.h"
#include "DirectX/DirectXCompatibleRenderTarget.h"
#include "Logging/LogMacros.h"
#include "Diagnostics/ScopedCycleCounter.h"
#include "SceneRendering/Scene.h"
#include "SceneRendering/DeferredSceneRenderer.h"

DEFINE_STATS_GROUP(DeferredGameViewport);

DeferredGameViewport::DeferredGameViewport(DirectXDeviceBundle* deviceBundle) : Super(deviceBundle)
{
	DirectXNew(hdrTarget, DirectXHDRTarget, deviceBundle);
}

DeferredGameViewport::~DeferredGameViewport()
{

}

void DeferredGameViewport::RenderScene(ID3D12GraphicsCommandList4* inCommandList, Scene* inScene)
{
	{
		QUICK_SCOPED_CYCLE_COUNTER(DeferredGameViewport, CalcVisibility);
		inScene->CalcVisibility();
	}

	{
		DeferredSceneRenderer renderer(inScene);
		renderer.RenderScene(inCommandList);
	}
}

void DeferredGameViewport::SetViewportResolution_Internal(int32 x, int32 y)
{
	Super::SetViewportResolution_Internal(x, y);

	if (x == 0 || y == 0) {
		return;
	}

	hdrTarget->ResizeBuffers(x, y);
}