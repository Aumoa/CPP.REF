// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Assets/Texture2D.h"
#include "GenericPlatform/GenericImage.h"
#include "Rendering/Texture2DTaskRenderAssetProxy.h"
#include "RHI/RHITexture2D.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"

bool ATexture2D::IsStreamedIn() const
{
	return RenderProxy && RenderProxy->TryResolve();
}

void ATexture2D::StreamIn()
{
	if (!RenderProxy)
	{
		RenderProxy = std::make_shared<NTexture2DTaskRenderAssetProxy>(LoadTextureAsync(AbsolutePath));
	}
}

void ATexture2D::StreamOut()
{
	RenderProxy.reset();
}

std::shared_ptr<NStreamableRenderAssetProxy> ATexture2D::GetRenderProxy() const
{
	return RenderProxy;
}

RefPtr<ATexture2D> ATexture2D::LoadAndStreamIn(String InAbsolutePath)
{
	auto Task = LoadTextureAsync(InAbsolutePath);
	auto Instance = ::NewObject<ATexture2D>();
	Instance->AbsolutePath = InAbsolutePath;
	Instance->StreamIn();
	return Instance;
}

Task<std::shared_ptr<NRHITexture2D>> ATexture2D::LoadTextureAsync(String InAbsolutePath)
{
	auto Image = co_await NGenericImage::LoadFromFileAsync(InAbsolutePath);
	auto Texture = co_await NRHIGlobal::GetDynamicRHI().CreateTexture2DAsync(Image);
	co_return Texture;
}