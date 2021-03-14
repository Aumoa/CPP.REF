// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXDeviceResource.h"

#include "DirectXMinimal.h"

class DirectXRaytracingShader;

class GAME_API DirectXShaderRecord : public DirectXDeviceResource
{
public:
	using Super = Object;

private:
	DirectXDeviceBundle* deviceBundle;
	TComPtr<ID3D12Resource> recordBuffer;
	void* mappingBuffer;
	size_t sizeInBytes;

public:
	DirectXShaderRecord(DirectXDeviceBundle* deviceBundle, const void* pShaderIdentifier);
	~DirectXShaderRecord() override;

	ID3D12Resource* GetBuffer() const;
	uint64 GetGPUVirtualAddress() const;
	uint64 GetSizeInBytes() const;

private:
	size_t CalcSizeInBytes(size_t paramCount) const;
	void Reallocate(size_t newSize);

private:
	std::vector<char> bytes;
};

class GAME_API DirectXShaderBindingTable : public DirectXDeviceResource
{
public:
	using Super = DirectXDeviceResource;

private:
	DirectXDeviceBundle* deviceBundle;
	TRefPtr<DirectXShaderRecord> rayGen;
	TRefPtr<DirectXShaderRecord> closestHit;
	TRefPtr<DirectXShaderRecord> miss;

public:
	DirectXShaderBindingTable(DirectXDeviceBundle* deviceBundle);
	~DirectXShaderBindingTable() override;

	void Init(DirectXRaytracingShader* initShader);
	void FillDispatchRaysDesc(D3D12_DISPATCH_RAYS_DESC& outDispatchRays) const;
};