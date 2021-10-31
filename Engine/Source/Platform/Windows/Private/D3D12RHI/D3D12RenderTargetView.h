// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12View.h"
#include "RHI/IRHIRenderTargetView.h"

class SD3D12RenderTargetView : public SD3D12View, implements IRHIRenderTargetView
{
	GENERATED_BODY(SD3D12RenderTargetView)

public:
	SD3D12RenderTargetView(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12DescriptorHeap> heap, size_t resources);

	virtual void CreateRenderTargetView(int32 index, IRHIResource* resource, const RHIRenderTargetViewDesc* desc) override;
};