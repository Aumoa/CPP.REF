// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXDeviceResource.h"

#include "DirectXMinimal.h"

class DirectXRaytracingShader;

class GAME_API DirectXShaderBindingTable : public DirectXDeviceResource
{
public:
	using Super = DirectXDeviceResource;

private:
	ID3D12Device5* device;
	TComPtr<ID3D12Resource> rayGeneration;

public:
	DirectXShaderBindingTable(DirectXDeviceBundle* deviceBundle);
	~DirectXShaderBindingTable() override;

	void Init(DirectXRaytracingShader* initShader);
	void FillDispatchRaysDesc(D3D12_DISPATCH_RAYS_DESC& outDispatchRays) const;

private:
	TComPtr<ID3D12Resource> CreateBuffer(uint64 sizeInBytes);
};