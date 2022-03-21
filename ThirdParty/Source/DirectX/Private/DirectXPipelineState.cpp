// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXPipelineState.h"

GENERATE_BODY(SDirectXPipelineState);

SDirectXPipelineState::SDirectXPipelineState(IRHIDevice* Owner, ComPtr<ID3D12PipelineState> pPipelineState)
	: Super(Owner)
	, pPipelineState(std::move(pPipelineState))
{
}