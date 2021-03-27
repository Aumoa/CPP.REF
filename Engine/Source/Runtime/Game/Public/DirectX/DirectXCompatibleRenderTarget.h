// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXRenderTarget.h"

#include "DirectX/DirectXMinimal.h"

class DirectXRenderTargetView;

class GAME_API DirectXCompatibleRenderTarget : public DirectXRenderTarget
{
public:
	using Super = DirectXRenderTarget;

private:
	TRefPtr<DirectXRenderTargetView> compatibleRTV;
	TComPtr<ID3D12Resource> compatibleBuffer;

public:
	DirectXCompatibleRenderTarget(DirectXDeviceBundle* deviceBundle);
	~DirectXCompatibleRenderTarget() override;

	void ResizeBuffers(int32 x, int32 y) override;

	DirectXRenderTargetView* GetRTV() const;
	ID3D12Resource* GetResource() const;
};