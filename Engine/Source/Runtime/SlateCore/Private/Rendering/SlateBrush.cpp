// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Rendering/SlateBrush.h"
#include "Rendering/Texture2DTaskRenderAssetProxy.h"
#include "Rendering/NullTextureRenderAssetProxy.h"

NSlateBrush::NSlateBrush(Task<std::shared_ptr<NRHITexture2D>> InTextureTask, Vector2 InDrawSize, Color InTintColor)
	: NSlateBrush(std::make_shared<NTexture2DTaskRenderAssetProxy>(InTextureTask), InDrawSize, InTintColor)
{
}

NSlateBrush::NSlateBrush(Vector2 InDrawSize, Color InTintColor)
	: NSlateBrush(std::make_shared<NNullTextureRenderAssetProxy>(), InDrawSize, InTintColor)
{
}

NSlateBrush::NSlateBrush(std::shared_ptr<NStreamableRenderAssetProxy> InRenderProxy, Vector2 InDrawSize, Color InTintColor)
	: RenderProxy(std::move(InRenderProxy))
	, DrawSize(InDrawSize)
	, TintColor(InTintColor)
{
}