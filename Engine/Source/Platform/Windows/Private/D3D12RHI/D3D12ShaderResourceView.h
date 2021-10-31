// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12View.h"
#include "RHI/IRHIShaderResourceView.h"

class SD3D12ShaderResourceView : public SD3D12View, implements IRHIShaderResourceView
{
	GENERATED_BODY(SD3D12ShaderResourceView)

public:
	SD3D12ShaderResourceView(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12DescriptorHeap> heap, size_t resources);

	virtual void CreateShaderResourceView(int32 index, IRHIResource* resource, const RHIShaderResourceViewDesc* desc) override;
};