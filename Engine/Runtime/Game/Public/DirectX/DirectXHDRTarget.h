// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXRenderTarget.h"

class DirectXShaderResourceView;

class GAME_API DirectXHDRTarget : public DirectXRenderTarget
{
public:
	using Super = DirectXRenderTarget;

private:
	TRefPtr<DirectXShaderResourceView> hdrUAV;
	TRefPtr<DirectXShaderResourceView> hdrSRV;
	TComPtr<ID3D12Resource> hdrBuffer;

public:
	DirectXHDRTarget(DirectXDeviceBundle* deviceBundle);
	~DirectXHDRTarget() override;

	void ResizeBuffers(int32 x, int32 y) override;

	DirectXShaderResourceView* GetUAV() const;
	DirectXShaderResourceView* GetSRV() const;
	ID3D12Resource* GetResource() const;
};