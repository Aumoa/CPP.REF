// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Assets/Texture2D.h"
#include "GameEngine.h"
#include "EngineSubsystems/GameRenderSystem.h"

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
}