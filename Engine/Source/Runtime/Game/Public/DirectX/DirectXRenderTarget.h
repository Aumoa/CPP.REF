// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXDeviceResource.h"

class GAME_API DirectXRenderTarget : public DirectXDeviceResource
{
public:
	using Super = DirectXDeviceResource;

private:
	ID3D12Device5* device;

public:
	DirectXRenderTarget(DirectXDeviceBundle* deviceBundle);
	~DirectXRenderTarget() override;

	virtual void ResizeBuffers(int32 x, int32 y) = 0;

protected:
	ID3D12Device5* GetDevice() const;

	TComPtr<ID3D12Resource> CreateRenderTexture2D(uint32 width, uint32 height, DXGI_FORMAT format, D3D12_RESOURCE_STATES initialState, bool bAllowRenderTarget, bool bAllowUnorderedAccess);
	TComPtr<ID3D12Resource> CreateDepthStencilTexture2D(uint32 width, uint32 height, DXGI_FORMAT format);
};