// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/DeferredRenderTarget.h"

#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"

DeferredRenderTarget::DeferredRenderTarget(DirectXDeviceBundle* deviceBundle) : Super()
	, deviceBundle(deviceBundle)
{

}

DeferredRenderTarget::~DeferredRenderTarget()
{

}

void DeferredRenderTarget::BeginDraw(ID3D12GraphicsCommandList4* inCommandList)
{

}

void DeferredRenderTarget::EndDraw(ID3D12GraphicsCommandList4* inCommandList)
{

}

void DeferredRenderTarget::ResizeBuffers(uint32 width, uint32 height)
{
	Super::ResizeBuffers(width, height);
}