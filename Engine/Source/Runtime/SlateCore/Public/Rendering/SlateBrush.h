// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Numerics/VectorInterface/Color.h"
#include "Rendering/StreamableRenderAssetProxy.h"

struct SLATECORE_API NSlateBrush
{
	Vector2 DrawSize = Vector2(32.0f, 32.0f);
	Color TintColor = NamedColors::White;
	std::shared_ptr<NStreamableRenderAssetProxy> RenderProxy;

	NSlateBrush() = default;
	NSlateBrush(Vector2 InDrawSize, Color InTintColor = NamedColors::White);
	NSlateBrush(Task<std::shared_ptr<NRHITexture2D>> InTextureTask, Vector2 InDrawSize = Vector2(32.0f, 32.0f), Color InTintColor = NamedColors::White);
	NSlateBrush(std::shared_ptr<NStreamableRenderAssetProxy> InRenderProxy, Vector2 InDrawSize = Vector2(32.0f, 32.0f), Color InTintColor = NamedColors::White);

	template<class U>
	NSlateBrush(const U& InRenderAssetProxy, Vector2 InDrawSize = Vector2(32.0f, 32.0f), Color InTintColor = NamedColors::White) requires
		requires
		{
			{ InRenderAssetProxy.GetRenderProxy() };
		}
		: NSlateBrush(InRenderAssetProxy.GetRenderProxy(), InDrawSize, InTintColor)
	{
	}

	template<class U>
	NSlateBrush(U& Ptr, Vector2 InDrawSize, Color InTintColor = NamedColors::White) requires
		requires
		{
			{ Ptr->GetRenderProxy() };
		}
		: NSlateBrush(Ptr ? Ptr->GetRenderProxy() : nullptr, InDrawSize, InTintColor)
	{
	}
};