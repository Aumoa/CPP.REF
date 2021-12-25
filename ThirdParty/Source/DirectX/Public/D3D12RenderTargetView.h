// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12View.h"
#include "RHI/IRHIRenderTargetView.h"

class DIRECTX_API SD3D12RenderTargetView : public SD3D12View, implements IRHIRenderTargetView
{
	GENERATED_BODY(SD3D12RenderTargetView)

public:
	SD3D12RenderTargetView(SDXGIFactory* InFactory, SD3D12Device* InDevice, ComPtr<ID3D12DescriptorHeap> Heap, size_t NumResources);

	virtual void CreateRenderTargetView(int32 Index, IRHIResource* Resource, const RHIRenderTargetViewDesc* Desc) override;
};