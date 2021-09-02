// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Assets/Texture2D.h"
#include "GameEngine.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "PlatformMisc/IPlatformImageLoader.h"
#include "PlatformMisc/IPlatformImage.h"

Texture2D::Texture2D(const std::filesystem::path& assetPath) : Super(assetPath)
{
}

Texture2D::~Texture2D()
{
}

void Texture2D::StreamIn()
{
	auto* renderSystem = GEngine->GetEngineSubsystem<GameRenderSystem>();
	RHIDevice* device = renderSystem->GetRHIDevice();
	IPlatformImageLoader& imageLoader = IPlatformImageLoader::Get();

	ScopedPtr<IPlatformImage> platformImage = imageLoader.LoadImageFromFile(GetAssetPath(), ERHIPixelFormat::B8G8R8A8_UNORM);
	_texture = device->CreateTexture2DFromImage(platformImage.Get());
}