// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Assets/Texture2D.h"
#include "GameEngine.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "PlatformMisc/IPlatformImageLoader.h"
#include "PlatformMisc/IPlatformImage.h"

STexture2D::STexture2D(const std::filesystem::path& assetPath) : Super(assetPath)
{
	_shaderResourceView = NewObject<SRHIShaderResourceView>(GEngine->GetEngineSubsystem<SGameRenderSystem>()->GetRHIDevice(), 1);
}

STexture2D::~STexture2D()
{
}

void STexture2D::StreamIn()
{
	auto* renderSystem = GEngine->GetEngineSubsystem<SGameRenderSystem>();
	SRHIDevice* device = renderSystem->GetRHIDevice();
	IPlatformImageLoader& imageLoader = IPlatformImageLoader::Get();

	ScopedPtr<IPlatformImage> platformImage = imageLoader.LoadImageFromFile(GetAssetPath(), ERHIPixelFormat::B8G8R8A8_UNORM);
	_texture = device->CreateTexture2DFromImage(platformImage.Get());
	_shaderResourceView->CreateShaderResourceView(_texture, 0);
}

Vector2N STexture2D::GetPixelSize() const
{
	if (_texture)
	{
		return _texture->GetPixelSize();
	}
	else
	{
		return Vector2N::GetZero();
	}
}