// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "SlateGlobalShaders.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"
#include "RHI/RHIRootSignature.h"
#include "RHI/RHIGraphicsPipelineState.h"

std::shared_ptr<NRHIRootSignature> NSlateGlobalShaders::SlateRS;
std::shared_ptr<NRHIGraphicsPipelineState> NSlateGlobalShaders::SlatePS;

void NSlateGlobalShaders::Initialize()
{
	auto& Graphics = NRHIGlobal::GetDynamicRHI();
	SlateRS = Graphics.CreateRootSignature();
	SlatePS = Graphics.CreateGraphicsPipelineState(*SlateRS);
}

void NSlateGlobalShaders::Shutdown()
{
	SlateRS.reset();
	SlatePS.reset();
}