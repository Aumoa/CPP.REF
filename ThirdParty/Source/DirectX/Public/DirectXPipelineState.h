// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "DirectXDeviceChild.h"

class DIRECTX_API SDirectXPipelineState : public SDirectXDeviceChild, implements IRHIPipelineState
{
	GENERATED_BODY(SDirectXPipelineState)

public:
	ComPtr<ID3D12PipelineState> pPipelineState;

public:
	SDirectXPipelineState(IRHIDevice* Owner, ComPtr<ID3D12PipelineState> pPipelineState);

	using Super::Dispose;
};