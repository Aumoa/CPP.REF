// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "RenderGlobal.h"
#include "Rendering/NullTextureRenderAssetProxy.h"
#include "Rendering/Texture2DTaskRenderAssetProxy.h"
#include "GenericPlatform/GenericImage.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"

std::shared_ptr<NStreamableRenderAssetProxy> NRenderGlobal::NullRenderProxy;

void NRenderGlobal::Initialize()
{
	NullRenderProxy = std::make_shared<NNullTextureRenderAssetProxy>();
}

void NRenderGlobal::Shutdown()
{
	NullRenderProxy.reset();
}

std::shared_ptr<NStreamableRenderAssetProxy> NRenderGlobal::LoadTexture2DAsync(String InFilename)
{
	auto LoadAsync = [InFilename]() -> Task<std::shared_ptr<NRHITexture2D>>
	{
		auto Image = co_await NGenericImage::LoadFromFileAsync(InFilename);
		auto Texture = co_await NRHIGlobal::GetDynamicRHI().CreateTexture2DAsync(Image);
		co_return Texture;
	};

	return std::make_shared<NTexture2DTaskRenderAssetProxy>(LoadAsync());
}