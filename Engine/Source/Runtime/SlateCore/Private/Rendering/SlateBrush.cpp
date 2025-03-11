// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/SlateBrush.h"
#include "Rendering/Texture2DTaskRenderAssetProxy.h"
#include "RenderGlobal.h"

NSlateBrush::NSlateBrush(Task<std::shared_ptr<NRHITexture2D>> InTextureTask, Vector2 InDrawSize, Color InTintColor)
	: NSlateBrush(std::make_shared<NTexture2DTaskRenderAssetProxy>(InTextureTask), InDrawSize, InTintColor)
{
}

NSlateBrush::NSlateBrush(Vector2 InDrawSize, Color InTintColor)
	: NSlateBrush(NRenderGlobal::GetNullRenderProxy(), InDrawSize, InTintColor)
{
}

NSlateBrush::NSlateBrush(std::shared_ptr<NStreamableRenderAssetProxy> InRenderProxy, Vector2 InDrawSize, Color InTintColor)
	: RenderProxy(InRenderProxy ? std::move(InRenderProxy) : NRenderGlobal::GetNullRenderProxy())
	, DrawSize(InDrawSize)
	, TintColor(InTintColor)
{
}