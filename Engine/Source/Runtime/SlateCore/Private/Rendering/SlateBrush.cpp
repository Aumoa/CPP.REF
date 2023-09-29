// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Rendering/SlateBrush.h"
#include "Rendering/Texture2DTaskRenderProxy.h"
#include "Rendering/NullTextureRenderProxy.h"

NSlateBrush::NSlateBrush(Task<std::shared_ptr<NRHITexture2D>> InTextureTask, Vector2 InDrawSize, Color InTintColor)
	: RenderProxy(std::make_shared<NTexture2DTaskRenderProxy>(InTextureTask))
	, DrawSize(InDrawSize)
	, TintColor(InTintColor)
{
}

NSlateBrush::NSlateBrush(Vector2 InDrawSize, Color InTintColor)
	: RenderProxy(std::make_shared<NNullTextureRenderProxy>())
	, DrawSize(InDrawSize)
	, TintColor(InTintColor)
{
}