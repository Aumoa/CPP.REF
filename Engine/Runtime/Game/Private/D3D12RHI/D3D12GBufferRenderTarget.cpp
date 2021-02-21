// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12GBufferRenderTarget.h"

#include "D3D12DeviceBundle.h"
#include "D3D12Resource.h"
#include "D3D12ShaderResourceView.h"

TRefPtr<IRHIRenderTarget> D3D12DeviceBundle::CreateGBufferRenderTarget()
{
	return NewObject<D3D12GBufferRenderTarget>(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

D3D12GBufferRenderTarget::D3D12GBufferRenderTarget(D3D12DeviceBundle* owner) : Super()
	, bundle(nullptr)
	, device(nullptr)

	, width(0)
	, height(0)

	, rtvIncrement(0)

	, renderPassRT{ }
	, renderPassDS{ }
{
	bundle = owner;
	device = bundle->Device;

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = { };
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = 2;
	HR(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeap)));
	rtvIncrement = device->GetDescriptorHandleIncrementSize(heapDesc.Type);

	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	heapDesc.NumDescriptors = 1;
	HR(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&dsvHeap)));

	srvHeap = NewObject<D3D12IndependentShaderResourceView>(device, 3);
}

D3D12GBufferRenderTarget::~D3D12GBufferRenderTarget()
{

}

void D3D12GBufferRenderTarget::ResizeBuffers(int32 width, int32 height)
{
	colorBuffer = CreateTexture2D(device, DXGI_FORMAT_B8G8R8A8_UNORM, D3D12_RESOURCE_STATE_COPY_SOURCE, width, height);
	normalBuffer = CreateTexture2D(device, DXGI_FORMAT_R16G16B16A16_UINT, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, width, height);
	depthStencilBuffer = CreateTexture2D(device, DXGI_FORMAT_R24G8_TYPELESS, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, width, height);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	uint32 increment = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	device->CreateRenderTargetView(colorBuffer->Resource, nullptr, rtvHandle);
	device->CreateRenderTargetView(normalBuffer->Resource, nullptr, rtvHandle.Get(increment, 1));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = { };
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	device->CreateDepthStencilView(depthStencilBuffer->Resource, &dsvDesc, dsvHandle);

	D3D12_SHADER_RESOURCE_VIEW_DESC ds_srv = { };
	ds_srv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	ds_srv.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	ds_srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	ds_srv.Texture2D.MipLevels = 1;

	srvHeap->CreateView(0, colorBuffer->Resource, nullptr);
	srvHeap->CreateView(1, normalBuffer->Resource, nullptr);
	srvHeap->CreateView(2, depthStencilBuffer->Resource, &ds_srv);

	this->width = width;
	this->height = height;

	D3D12_CLEAR_VALUE clearValue = { };
	clearValue.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	renderPassRT[0].cpuDescriptor = rtvHandle;
	renderPassRT[0].BeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
	renderPassRT[0].BeginningAccess.Clear.ClearValue = clearValue;
	renderPassRT[0].EndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;

	clearValue.Format = DXGI_FORMAT_R16G16B16A16_UINT;
	renderPassRT[1].cpuDescriptor = rtvHandle.Get(increment, 1);
	renderPassRT[1].BeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
	renderPassRT[1].BeginningAccess.Clear.ClearValue = clearValue;
	renderPassRT[1].EndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;

	clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	clearValue.DepthStencil.Depth = 1.0f;
	renderPassDS.cpuDescriptor = dsvHandle;
	renderPassDS.DepthBeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
	renderPassDS.DepthBeginningAccess.Clear.ClearValue = clearValue;
	renderPassDS.DepthEndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;
	renderPassDS.StencilBeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
	renderPassDS.StencilBeginningAccess.Clear.ClearValue = clearValue;
	renderPassDS.StencilEndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;
}

size_t D3D12GBufferRenderTarget::GetRenderTargetCount() const
{
	return 2;
}

IRHIResource* D3D12GBufferRenderTarget::GetRenderTarget(size_t index) const
{
	IRHIResource* resources[] = { colorBuffer.Get(), normalBuffer.Get() };
	return resources[index];
}

IRHIShaderResourceView* D3D12GBufferRenderTarget::GetShaderResourceView() const
{
	return srvHeap.Get();
}

IRHIUnorderedAccessView* D3D12GBufferRenderTarget::GetUnorderedAccessView() const
{
	return nullptr;
}

uint32 D3D12GBufferRenderTarget::Width_get() const
{
	return (uint32)width;
}

uint32 D3D12GBufferRenderTarget::Height_get() const
{
	return (uint32)height;
}

void D3D12GBufferRenderTarget::BeginRender(ID3D12GraphicsCommandList4* inCommandList)
{
	D3D12_RESOURCE_BARRIER barriers[3] =
	{
		TRANSITION(colorBuffer, COPY_SOURCE, RENDER_TARGET),
		TRANSITION(normalBuffer, PIXEL_SHADER_RESOURCE, RENDER_TARGET),
		TRANSITION(depthStencilBuffer, PIXEL_SHADER_RESOURCE, DEPTH_WRITE)
	};

	inCommandList->ResourceBarrier(3, barriers);
	inCommandList->BeginRenderPass(2, renderPassRT, &renderPassDS, D3D12_RENDER_PASS_FLAG_NONE);

	D3D12_VIEWPORT vp = { };
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MaxDepth = 1.0f;

	D3D12_RECT sc = { };
	sc.right = width;
	sc.bottom = height;

	inCommandList->RSSetViewports(1, &vp);
	inCommandList->RSSetScissorRects(1, &sc);
}

void D3D12GBufferRenderTarget::EndRender(ID3D12GraphicsCommandList4* inCommandList)
{
	D3D12_RESOURCE_BARRIER barriers[3] =
	{
		TRANSITION(colorBuffer, RENDER_TARGET, PIXEL_SHADER_RESOURCE),
		TRANSITION(normalBuffer, RENDER_TARGET, PIXEL_SHADER_RESOURCE),
		TRANSITION(depthStencilBuffer, DEPTH_WRITE, PIXEL_SHADER_RESOURCE)
	};

	inCommandList->EndRenderPass();
	inCommandList->ResourceBarrier(3, barriers);
}