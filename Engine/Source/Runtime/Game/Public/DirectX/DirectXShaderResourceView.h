// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXView.h"

class GAME_API DirectXShaderResourceView : public DirectXView
{
public:
	using Super = DirectXView;

private:
	ID3D12Device5* device;

public:
	DirectXShaderResourceView(DirectXDeviceBundle* deviceBundle, uint32 numRTVs);
	~DirectXShaderResourceView() override;

	void CreateConstantBufferView(size_t index, uint64 bufferLocation, uint32 sizeInBytes);
	void CreateShaderResourceView(size_t index, ID3D12Resource* srvTarget, const D3D12_SHADER_RESOURCE_VIEW_DESC* srvDesc);
	void CreateUnorderedAccessView(size_t index, ID3D12Resource* uavTarget, ID3D12Resource* counterResource, const D3D12_UNORDERED_ACCESS_VIEW_DESC* uavDesc);
};