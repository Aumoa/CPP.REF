// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIShaderResourceView.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Common.h"

class ND3D12ShaderResourceView : public NRHIShaderResourceView
{
	ID3D12Device* const pDevice;
	const size_t NumViews;
	ComPtr<ID3D12DescriptorHeap> pHeap;

public:
	ND3D12ShaderResourceView(ID3D12Device& InDevice, size_t InNumViews);

	virtual size_t GetNumViews() const override { return NumViews; }
	virtual void CreateView(size_t Index, NRHITexture2D* InResource) override;

	inline ID3D12DescriptorHeap* Get() const { return pHeap.Get(); }
};

#endif