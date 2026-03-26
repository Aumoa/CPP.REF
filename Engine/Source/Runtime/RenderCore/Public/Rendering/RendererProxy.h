// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	class RENDERCORE_API PLATFORM_NOVTABLE RendererProxy
	{
	protected:
		RendererProxy();

	public:
		virtual ~RendererProxy() noexcept;
	};
}