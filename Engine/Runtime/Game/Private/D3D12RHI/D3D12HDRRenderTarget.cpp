// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12HDRRenderTarget.h"

#include "D3D12DeviceBundle.h"
#include "D3D12ShaderResourceView.h"
#include "D3D12UnorderedAccessView.h"
#include "D3D12Resource.h"

TRefPtr<IRHIRenderTarget> D3D12DeviceBundle::CreateHDRRenderTarget()
{
	return NewObject<D3D12HDRRenderTarget>(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

D3D12HDRRenderTarget::D3D12HDRRenderTarget(D3D12DeviceBundle* owner) : Super()
	, bundle(nullptr)
	, device(nullptr)

	, width(0)
	, height(0)
{
	bundle = owner;
	device = bundle->Device;

	srvHeap = NewObject<D3D12IndependentShaderResourceView>(device, 1);
	uavHeap = NewObject<D3D12IndependentUnorderedAccessView>(device, 1);
}

D3D12HDRRenderTarget::~D3D12HDRRenderTarget()
{

}

void D3D12HDRRenderTarget::ResizeBuffers(int32 width, int32 height)
{
	colorBuffer = CreateTexture2D(device, DXGI_FORMAT_R16G16B16A16_FLOAT, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, width, height, true);

	srvHeap->CreateView(0, colorBuffer->Resource, nullptr);
	uavHeap->CreateView(0, colorBuffer->Resource, nullptr);

	this->width = width;
	this->height = height;
}

size_t D3D12HDRRenderTarget::GetRenderTargetCount() const
{
	return 1;
}

IRHIResource* D3D12HDRRenderTarget::GetRenderTarget(size_t index) const
{
	return colorBuffer.Get();
}

IRHIShaderResourceView* D3D12HDRRenderTarget::GetShaderResourceView() const
{
	return srvHeap.Get();
}

IRHIUnorderedAccessView* D3D12HDRRenderTarget::GetUnorderedAccessView() const
{
	return uavHeap.Get();
}

uint32 D3D12HDRRenderTarget::Width_get() const
{
	return (uint32)width;
}

uint32 D3D12HDRRenderTarget::Height_get() const
{
	return (uint32)height;
}

void D3D12HDRRenderTarget::BeginRender(ID3D12GraphicsCommandList4* inCommandList)
{
	D3D12_RESOURCE_BARRIER barriers[1] =
	{
		TRANSITION(colorBuffer, PIXEL_SHADER_RESOURCE, RENDER_TARGET),
	};

	inCommandList->ResourceBarrier(1, barriers);
	//inCommandList->BeginRenderPass(1, &renderPass, nullptr, D3D12_RENDER_PASS_FLAG_NONE);
}

void D3D12HDRRenderTarget::EndRender(ID3D12GraphicsCommandList4* inCommandList)
{
	D3D12_RESOURCE_BARRIER barriers[1] =
	{
		TRANSITION(colorBuffer, RENDER_TARGET, PIXEL_SHADER_RESOURCE),
	};

	//inCommandList->EndRenderPass();
	inCommandList->ResourceBarrier(1, barriers);
}