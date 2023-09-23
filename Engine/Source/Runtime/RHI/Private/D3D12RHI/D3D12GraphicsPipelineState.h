// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIGraphicsPipelineState.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Common.h"

class ND3D12GraphicsPipelineState : public NRHIGraphicsPipelineState
{
	ComPtr<ID3D12PipelineState> pPS;

public:
	ND3D12GraphicsPipelineState(ID3D12Device& InDevice, ID3D12RootSignature& RS);

	inline ID3D12PipelineState* Get() const { return pPS.Get(); }
};

#endif