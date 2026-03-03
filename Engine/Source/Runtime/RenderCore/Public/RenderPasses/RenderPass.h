// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	class CommandBuffer;

	class RENDERCORE_API RenderPass
	{
	public:
		virtual ~RenderPass() noexcept;

		virtual void Setup() = 0;
		virtual void Execute(CommandBuffer* commandBuffer) = 0;
	};
}