// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DeferredGameViewport.h"

#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXHDRTarget.h"
#include "DirectX/DirectXCompatibleRenderTarget.h"
#include "DirectX/DirectXDeviceContext.h"
#include "Logging/LogMacros.h"
#include "Diagnostics/ScopedCycleCounter.h"
#include "SceneRendering/Scene.h"
#include "SceneRendering/RaytracingSceneRenderer.h"

DEFINE_STATS_GROUP(DeferredGameViewport);

DeferredGameViewport::DeferredGameViewport(DirectXDeviceBundle* deviceBundle) : Super(deviceBundle)
{
	DirectXNew(hdrTarget, DirectXHDRTarget, deviceBundle);
}

DeferredGameViewport::~DeferredGameViewport()
{

}

void DeferredGameViewport::RenderScene(DirectXDeviceContext* deviceContext, Scene* inScene)
{
	ID3D12GraphicsCommandList4* inCommandList = deviceContext->GetCommandList();

	{
		QUICK_SCOPED_CYCLE_COUNTER(DeferredGameViewport, InitViews);
		inScene->InitViews();
	}

	{
		inScene->BeginRender(deviceContext);

		D3D12_RESOURCE_BARRIER barrier = { };
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = hdrTarget->GetResource();
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		inCommandList->ResourceBarrier(1, &barrier);

		RaytracingSceneRenderer renderer(inScene);
		renderer.InitGlobalInputs({ hdrTarget->GetUAV() });
		renderer.RenderScene(inCommandList, this);

		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;
		inCommandList->ResourceBarrier(1, &barrier);

		inScene->EndRender(deviceContext);
	}

	auto r = GetCompatibleRenderTarget();
	auto dst = r->GetResource();

	D3D12_RESOURCE_BARRIER barrier = { };
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = dst;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;

	inCommandList->ResourceBarrier(1, &barrier);
	inCommandList->CopyResource(r->GetResource(), hdrTarget->GetResource());

	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;
	inCommandList->ResourceBarrier(1, &barrier);
}

void DeferredGameViewport::SetViewportResolution_Internal(int32 x, int32 y)
{
	Super::SetViewportResolution_Internal(x, y);
	hdrTarget->ResizeBuffers(x, y);
}