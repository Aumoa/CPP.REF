// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Numerics/VectorInterface/Color.h"
#include "Rendering/SlateRenderProxy.h"

struct SLATECORE_API NSlateBrush
{
	Vector2 DrawSize = Vector2(32.0f, 32.0f);
	Color TintColor = NamedColors::White;
	std::shared_ptr<NSlateRenderProxy> RenderProxy;

	NSlateBrush() = default;
	NSlateBrush(Task<std::shared_ptr<NRHITexture2D>> InTextureTask, Vector2 InDrawSize = Vector2(32.0f, 32.0f), Color InTintColor = NamedColors::White);
};