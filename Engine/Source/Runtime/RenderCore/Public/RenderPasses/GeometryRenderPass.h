// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RenderPasses/RenderPass.h"

namespace Ayla
{
	class RENDERCORE_API GeometryRenderPass : public RenderPass
	{
	public:
		GeometryRenderPass();
		virtual ~GeometryRenderPass() noexcept override;

		virtual void Setup() override;
		virtual void Execute(CommandBuffer* commandBuffer) override;
	};
}