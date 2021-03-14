// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXShaderBindingTable.h"

#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "DirectX/DirectXRaytracingShader.h"

using namespace std;

DirectXShaderRecord::DirectXShaderRecord(DirectXDeviceBundle* deviceBundle, const void* pShaderIdentifier) : Super()
	, deviceBundle(deviceBundle)
	, mappingBuffer(nullptr)
	, sizeInBytes(0)
{
	Reallocate(CalcSizeInBytes(0));
	memcpy(mappingBuffer, pShaderIdentifier, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
}

DirectXShaderRecord::~DirectXShaderRecord()
{

}

ID3D12Resource* DirectXShaderRecord::GetBuffer() const
{
	return recordBuffer.Get();
}

uint64 DirectXShaderRecord::GetGPUVirtualAddress() const
{
	return recordBuffer->GetGPUVirtualAddress();
}

uint64 DirectXShaderRecord::GetSizeInBytes() const
{
	return sizeInBytes;
}

size_t DirectXShaderRecord::CalcSizeInBytes(size_t paramCount) const
{
	return D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES + sizeof(D3D12_GPU_VIRTUAL_ADDRESS) * paramCount;
}

void DirectXShaderRecord::Reallocate(size_t newSize)
{
	// Need reallocate.
	if (sizeInBytes < newSize)
	{
		TComPtr<ID3D12Resource> newBuffer = deviceBundle->CreateDynamicBuffer(newSize);
		void* newBufferMap = nullptr;
		HR(newBuffer->Map(0, nullptr, &newBufferMap));
		
		// Copy previous bytes to new buffer.
		memcpy(newBufferMap, mappingBuffer, sizeInBytes);

		// Replace binding table.
		recordBuffer = move(newBuffer);
		mappingBuffer = newBufferMap;

		SetDeviceChildPtr(recordBuffer.Get(), deviceBundle);
	}
}

// ^^^^^^^^^^^^ ShaderRecord ^^^^^^^^^^^^vvvvvvvvvvvv DirectXShaderBindingTable

DirectXShaderBindingTable::DirectXShaderBindingTable(DirectXDeviceBundle* deviceBundle) : Super()
	, deviceBundle(deviceBundle)
{
	SetDeviceChildPtr(nullptr, deviceBundle);
}

DirectXShaderBindingTable::~DirectXShaderBindingTable()
{

}

void DirectXShaderBindingTable::Init(DirectXRaytracingShader* initShader)
{
	DirectXNew(rayGen, DirectXShaderRecord, deviceBundle, initShader->GetRayGenerationIdentifier());
	DirectXNew(closestHit, DirectXShaderRecord, deviceBundle, initShader->GetClosestHitIdentifier(0));
	DirectXNew(miss, DirectXShaderRecord, deviceBundle, initShader->GetMissIdentifier(0));
}

void DirectXShaderBindingTable::FillDispatchRaysDesc(D3D12_DISPATCH_RAYS_DESC& outDispatchRays) const
{
	outDispatchRays.RayGenerationShaderRecord.StartAddress = rayGen->GetGPUVirtualAddress();
	outDispatchRays.RayGenerationShaderRecord.SizeInBytes = rayGen->GetSizeInBytes();
	outDispatchRays.HitGroupTable.StartAddress = closestHit->GetGPUVirtualAddress();
	outDispatchRays.HitGroupTable.SizeInBytes = closestHit->GetSizeInBytes();
	outDispatchRays.HitGroupTable.StrideInBytes = closestHit->GetSizeInBytes();
	outDispatchRays.MissShaderTable.StartAddress = miss->GetGPUVirtualAddress();
	outDispatchRays.MissShaderTable.SizeInBytes = miss->GetSizeInBytes();
	outDispatchRays.MissShaderTable.StrideInBytes = miss->GetSizeInBytes();
}