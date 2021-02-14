// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12GBufferRenderTarget.h"

#include "Engine.h"
#include "D3D12DeviceBundle.h"
#include "D3D12CommandList.h"
#include "D3D12ShaderResourceView.h"
#include "D3D12Resource.h"

using namespace std;

D3D12GBufferRenderTarget::D3D12GBufferRenderTarget() : Super()
	, bundle(nullptr)
	, device(nullptr)

	, width(0)
	, height(0)
	
	, rtvIncrement(0)
{
	bundle = Cast<D3D12DeviceBundle>(GEngine.DeviceBundle);
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
	colorBuffer = CreateTexture2D(DXGI_FORMAT_B8G8R8A8_UNORM, D3D12_RESOURCE_STATE_COPY_SOURCE, width, height);
	normalBuffer = CreateTexture2D(DXGI_FORMAT_R16G16B16A16_UINT, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, width, height);
	depthStencilBuffer = CreateTexture2D(DXGI_FORMAT_R24G8_TYPELESS, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, width, height);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	uint32 increment = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	device->CreateRenderTargetView(colorBuffer->Resource, nullptr, rtvHandle.IncrementPost(increment));
	device->CreateRenderTargetView(normalBuffer->Resource, nullptr, rtvHandle.IncrementPost(increment));

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

void D3D12GBufferRenderTarget::BeginRender(ID3D12GraphicsCommandList* inCommandList)
{
	D3D12_RESOURCE_BARRIER barriers[3] =
	{
		TRANSITION(colorBuffer, COPY_SOURCE, RENDER_TARGET),
		TRANSITION(normalBuffer, PIXEL_SHADER_RESOURCE, RENDER_TARGET),
		TRANSITION(depthStencilBuffer, PIXEL_SHADER_RESOURCE, DEPTH_WRITE)
	};

	depthStencilBuffer->Resource->SetName(L"depthStencilBuffer");
	inCommandList->ResourceBarrier(3, barriers);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	auto dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { rtvHandle.Get(rtvIncrement, 0), rtvHandle.Get(rtvIncrement, 1) };

	inCommandList->OMSetRenderTargets(2, handles, FALSE, &dsvHandle);

	FLOAT transparent[4] = { };
	inCommandList->ClearRenderTargetView(rtvHandle.Get(rtvIncrement, 0), transparent, 0, nullptr);
	inCommandList->ClearRenderTargetView(rtvHandle.Get(rtvIncrement, 1), transparent, 0, nullptr);
	inCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

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

void D3D12GBufferRenderTarget::EndRender(ID3D12GraphicsCommandList* inCommandList)
{
	D3D12_RESOURCE_BARRIER barriers[3] =
	{
		TRANSITION(colorBuffer, RENDER_TARGET, PIXEL_SHADER_RESOURCE),
		TRANSITION(normalBuffer, RENDER_TARGET, PIXEL_SHADER_RESOURCE),
		TRANSITION(depthStencilBuffer, DEPTH_WRITE, PIXEL_SHADER_RESOURCE)
	};

	inCommandList->ResourceBarrier(3, barriers);
}

TRefPtr<D3D12Resource> D3D12GBufferRenderTarget::CreateTexture2D(DXGI_FORMAT format, D3D12_RESOURCE_STATES initialState, int32 width, int32 height)
{
	D3D12_RESOURCE_DESC textureDesc = { };
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureDesc.Width = (uint64)width;
	textureDesc.Height = (uint32)height;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = format;
	textureDesc.SampleDesc = { 1, 0 };
	textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	D3D12_HEAP_PROPERTIES heapProp = { };
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_CLEAR_VALUE clearValue = { };
	clearValue.Format = format;

	// Is depth stencil buffer format.
	// IMPORTANT WARNING: Internal only!!
	if (format == DXGI_FORMAT_R24G8_TYPELESS)
	{
		textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		clearValue.DepthStencil.Depth = 1.0f;
	}

	ComPtr<ID3D12Resource> resource;
	HR(device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &textureDesc, initialState, &clearValue, IID_PPV_ARGS(&resource)));

	return NewObject<D3D12Resource>(resource.Get());
}