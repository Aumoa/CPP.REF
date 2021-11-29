// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12View.h"
#include "RHI/IRHIDepthStencilView.h"

class SD3D12DepthStencilView : public SD3D12View, implements IRHIDepthStencilView
{
	GENERATED_BODY(SD3D12DepthStencilView)

public:
	SD3D12DepthStencilView(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12DescriptorHeap> heap, size_t resources);

	virtual void CreateDepthStencilView(int32 index, IRHIResource* resource, const RHIDepthStencilViewDesc* desc) override;
};