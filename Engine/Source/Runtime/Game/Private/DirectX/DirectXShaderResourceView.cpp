// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXShaderResourceView.h"

#include "DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "Logging/LogMacros.h"

DirectXShaderResourceView::DirectXShaderResourceView(DirectXDeviceBundle* deviceBundle, uint32 numRTVs) : Super(deviceBundle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, numRTVs)
	, device(deviceBundle->GetDevice())
{

}

DirectXShaderResourceView::~DirectXShaderResourceView()
{

}

void DirectXShaderResourceView::CreateConstantBufferView(size_t index, uint64 bufferLocation, uint32 sizeInBytes)
{
	if (index >= NumDescriptors)
	{
		SE_LOG(LogDirectX, Error, L"CreateConstantBufferView: index is over than NumDescriptors.");
		return;
	}

	D3D12_CONSTANT_BUFFER_VIEW_DESC view = { };
	view.BufferLocation = bufferLocation;
	view.SizeInBytes = sizeInBytes;

	D3D12_CPU_DESCRIPTOR_HANDLE handle = GetCPUHandle(index);
	device->CreateConstantBufferView(&view, handle);
}

void DirectXShaderResourceView::CreateShaderResourceView(size_t index, ID3D12Resource* srvTarget, const D3D12_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	if (index >= NumDescriptors)
	{
		SE_LOG(LogDirectX, Error, L"CreateShaderResourceView: index is over than NumDescriptors.");
		return;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE handle = GetCPUHandle(index);
	device->CreateShaderResourceView(srvTarget, srvDesc, handle);
}

void DirectXShaderResourceView::CreateUnorderedAccessView(size_t index, ID3D12Resource* uavTarget, ID3D12Resource* counterResource, const D3D12_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	if (index >= NumDescriptors)
	{
		SE_LOG(LogDirectX, Error, L"CreateUnorderedAccessView: index is over than NumDescriptors.");
		return;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE handle = GetCPUHandle(index);
	device->CreateUnorderedAccessView(uavTarget, counterResource, uavDesc, handle);
}