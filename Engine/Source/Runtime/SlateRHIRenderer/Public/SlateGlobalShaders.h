// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class NRHIRootSignature;
class NRHIGraphicsPipelineState;

struct SLATERHIRENDERER_API NSlateGlobalShaders : public StaticClass
{
	static void Initialize();
	static void Shutdown();

	static NRHIRootSignature& GetSlateRootSignature() { return *SlateRS; }
	static NRHIGraphicsPipelineState& GetSlatePipelineState() { return *SlatePS; }

private:
	static std::shared_ptr<NRHIRootSignature> SlateRS;
	static std::shared_ptr<NRHIGraphicsPipelineState> SlatePS;
};