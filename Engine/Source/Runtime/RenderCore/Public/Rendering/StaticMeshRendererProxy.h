// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Rendering/RendererProxy.h"

namespace Ayla
{
	class RENDERCORE_API StaticMeshRendererProxy : public RendererProxy
	{
	public:
		StaticMeshRendererProxy();
		virtual ~StaticMeshRendererProxy() noexcept override;
	};
}