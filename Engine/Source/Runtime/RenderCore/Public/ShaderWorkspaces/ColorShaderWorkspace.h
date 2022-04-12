// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CommonShaderWorkspace.h"

namespace libty::inline RenderCore
{
	class RENDERCORE_API SColorShaderWorkspace : public SCommonShaderWorkspace
	{
		GENERATED_BODY(SColorShaderWorkspace);

	public:
		SColorShaderWorkspace(IRHIDevice* device);

	protected:
		virtual IRHIRootSignature* CreateRootSignature(IRHIShaderCodeWorkspace* workspace) override;
		virtual IRHIPipelineState* CreateGraphicsPipelineState(IRHIShaderCodeWorkspace* workspace, IRHIRootSignature* signature) override;
	};
}