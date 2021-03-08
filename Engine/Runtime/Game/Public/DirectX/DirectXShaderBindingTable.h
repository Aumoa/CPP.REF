// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXDeviceResource.h"

#include "DirectXMinimal.h"

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

private:
	TComPtr<ID3D12Resource> CreateBuffer(uint64 sizeInBytes);
};