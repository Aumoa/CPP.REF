// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXShaderBindingTable.h"

#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "DirectX/DirectXRaytracingShader.h"

using namespace std;

DirectXShaderRecord::DirectXShaderRecord(const DirectXShaderRecordInfo& sInfo) : Super()
	, shaderIdentifier(sInfo.ShaderIdentifier)
	, sizeInBytes(CalcSizeInBytes(sInfo.NumParameters))
{
	params.resize(sInfo.NumParameters);
}

DirectXShaderRecord::~DirectXShaderRecord()
{

}

uint64 DirectXShaderRecord::GetSizeInBytes() const
{
	return sizeInBytes;
}

void DirectXShaderRecord::SetParameter(uint32 index, uint64 handle)
{
	params[index] = handle;
}

void DirectXShaderRecord::Update(char* destination)
{
	memcpy(destination, shaderIdentifier, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
	memcpy(destination + D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES, params.data(), sizeof(uint64) * params.size());
}

size_t DirectXShaderRecord::CalcSizeInBytes(size_t paramCount) const
{
	size_t actualSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES + sizeof(D3D12_GPU_VIRTUAL_ADDRESS) * paramCount;
	return RoundUp(actualSize, D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT);
}

size_t DirectXShaderRecord::RoundUp(size_t actual, size_t powerOf2Alignment)
{
	powerOf2Alignment -= 1;
	return (actual + powerOf2Alignment) & ~powerOf2Alignment;
}

// ^^^^^^^^^^^^ ShaderRecord ^^^^^^^^^^^^vvvvvvvvvvvv DirectXInstanceShaderRecord vvvvvvvvvvvv

DirectXInstanceShaderRecord::DirectXInstanceShaderRecord(size_t shaderIndex, const vector<uint64>& inParams)
	: ShaderIndex(shaderIndex)
	, RootParameters(inParams)
{

}

DirectXInstanceShaderRecord::DirectXInstanceShaderRecord(size_t shaderIndex, vector<uint64>&& inParams)
	: ShaderIndex(shaderIndex)
	, RootParameters(move(inParams))
{

}

// ^^^^^^^^^^^^ DirectXInstanceShaderRecord ^^^^^^^^^^^^vvvvvvvvvvvv DirectXShaderBindingTable vvvvvvvvvvvv

DirectXShaderBindingTable::DirectXShaderBindingTable(DirectXDeviceBundle* deviceBundle) : Super()
	, deviceBundle(deviceBundle)
	, shader(nullptr)

	, rayGenLen(0)

	, closestHitStride(0)
	, closestHitLen(0)

	, missStride(0)
	, missLen(0)
{
	SetDeviceChildPtr(nullptr, deviceBundle);
}

DirectXShaderBindingTable::~DirectXShaderBindingTable()
{

}

void DirectXShaderBindingTable::Init(DirectXRaytracingShader* initShader, span<DirectXInstanceShaderRecord const> hitGroupInstances, span<DirectXInstanceShaderRecord const> missInstances)
{
	shader = initShader;

	rayGen = NewObject<DirectXShaderRecord>(initShader->GetRayGenerationRecord());

	// Get ready shader identifiers.
	vector<DirectXShaderRecordInfo> closestRecords = initShader->GetClosestHitRecords();
	vector<DirectXShaderRecordInfo> missRecords = initShader->GetMissRecords();

	// Add instances for each shader identifier.
	closestHit.resize(0);
	for (size_t i = 0; i < hitGroupInstances.size(); ++i)
	{
		AddShaderRecord(closestHit, closestRecords, hitGroupInstances[i]);
	}
	miss.resize(0);
	for (size_t i = 0; i < missRecords.size(); ++i)
	{
		AddShaderRecord(miss, missRecords, missInstances[i]);
	}

	// Update gpu upload buffer.
	ReadyBuffers();
	Update();
}

void DirectXShaderBindingTable::FillDispatchRaysDesc(D3D12_DISPATCH_RAYS_DESC& outDispatchRays) const
{
	outDispatchRays.RayGenerationShaderRecord.StartAddress = rayGenBuf->GetGPUVirtualAddress();
	outDispatchRays.RayGenerationShaderRecord.SizeInBytes = rayGenLen;
	outDispatchRays.HitGroupTable.StartAddress = closestHitBuf->GetGPUVirtualAddress();
	outDispatchRays.HitGroupTable.SizeInBytes = closestHitLen;
	outDispatchRays.HitGroupTable.StrideInBytes = closestHitStride;
	outDispatchRays.MissShaderTable.StartAddress = missBuf->GetGPUVirtualAddress();
	outDispatchRays.MissShaderTable.SizeInBytes = missLen;
	outDispatchRays.MissShaderTable.StrideInBytes = missStride;
}

void DirectXShaderBindingTable::Update()
{
	// Update shader binding table records.
	rayGen->Update(rayGenBufPtr);

	char* ptr = closestHitBufPtr;
	for (size_t i = 0; i < closestHit.size(); ++i)
	{
		closestHit[i]->Update(ptr);
		ptr += closestHitStride;
	}

	ptr = missBufPtr;
	for (size_t i = 0; i < miss.size(); ++i)
	{
		miss[i]->Update(ptr);
		ptr += missStride;
	}
}

void DirectXShaderBindingTable::AddShaderRecord(vector<TRefPtr<DirectXShaderRecord>>& target, const vector<DirectXShaderRecordInfo>& identifiers, const DirectXInstanceShaderRecord& instance)
{
	size_t shaderIndex = instance.ShaderIndex;
	DirectXShaderRecord* record = target.emplace_back(NewObject<DirectXShaderRecord>(identifiers[shaderIndex])).Get();

	for (size_t i = 0; i < instance.RootParameters.size(); ++i)
	{
		record->SetParameter((uint32)i, instance.RootParameters[i]);
	}
}

void DirectXShaderBindingTable::ReadyBuffers()
{
	uint64 _;
	ReadyBufferSingle({ rayGen }, rayGenBuf, rayGenLen, _, rayGenBufPtr);
	ReadyBufferSingle(closestHit, closestHitBuf, closestHitLen, closestHitStride, closestHitBufPtr);
	ReadyBufferSingle(miss, missBuf, missLen, missStride, missBufPtr);
}

void DirectXShaderBindingTable::ReadyBufferSingle(const vector<TRefPtr<DirectXShaderRecord>>& records, TComPtr<ID3D12Resource>& buf, uint64& length, uint64& stride, char*& bufPtr)
{
	// Calc stride.
	{
		stride = 0;
		for (size_t i = 0; i < records.size(); ++i)
		{
			stride = max(stride, records[i]->GetSizeInBytes());
		}
	}

	// Calc full length.
	size_t new_length = stride * records.size();
	if (new_length > length)
	{
		// Create buffer and copy bytes.
		buf = deviceBundle->CreateDynamicBuffer((size_t)new_length);
		HR(buf->Map(0, nullptr, (void**)&bufPtr));
		length = new_length;
	}
}