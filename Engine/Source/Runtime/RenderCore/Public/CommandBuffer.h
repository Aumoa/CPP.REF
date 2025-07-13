// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	class RENDERCORE_API CommandBuffer
	{
	protected:
		CommandBuffer();

	public:
		virtual ~CommandBuffer() noexcept;

		virtual void BeginCommands() = 0;
		virtual void EndCommands() = 0;
	};
}