// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXView.h"

class GAME_API DirectXRenderTargetView : public DirectXView
{
public:
	using Super = DirectXView;

private:
	ID3D12Device5* device;

public:
	DirectXRenderTargetView(DirectXDeviceBundle* deviceBundle, uint32 numRTVs);
	~DirectXRenderTargetView() override;

	void CreateView(size_t index, ID3D12Resource* rtvTarget, const D3D12_RENDER_TARGET_VIEW_DESC* rtvDesc);
};