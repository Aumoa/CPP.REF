// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXView.h"

class GAME_API DirectXDepthStencilView : public DirectXView
{
public:
	using Super = DirectXView;

private:
	ID3D12Device5* device;

public:
	DirectXDepthStencilView(DirectXDeviceBundle* deviceBundle, uint32 numRTVs);
	~DirectXDepthStencilView() override;

	void CreateView(size_t index, ID3D12Resource* rtvTarget, const D3D12_DEPTH_STENCIL_VIEW_DESC* rtvDesc);
};