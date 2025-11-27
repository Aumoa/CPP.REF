// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/StreamableRenderAsset.h"
#include "Graphics.h"

namespace Ayla
{
	StreamableRenderAsset::StreamableRenderAsset(SharedPtr<Graphics> graphics)
		: m_Graphics(std::move(graphics))
	{
	}

	StreamableRenderAsset::~StreamableRenderAsset() noexcept
	{
	}

	SharedPtr<Graphics> StreamableRenderAsset::GetGraphics()
	{
		return m_Graphics;
	}
}