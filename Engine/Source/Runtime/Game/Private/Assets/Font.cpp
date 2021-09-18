// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Assets/Font.h"
#include "GameEngine.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "FreeType/FreeTypeModule.h"

SFont::SFont(const std::filesystem::path& assetPath) : Super(assetPath)
{
}

SFont::~SFont()
{
}

void SFont::StreamIn()
{
	auto* renderSystem = GEngine->GetEngineSubsystem<SGameRenderSystem>();
	SFreeTypeModule* module = renderSystem->GetFreeTypeModule();
	_fontFace = module->CreateFontFace(GetAssetPath(), 0);
}