// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameInstance.h"
#include "GameEngine.h"
#include "Level/Level.h"
#include "EngineSubsystems/GameLevelSystem.h"
#include "Application/SlateApplication.h"

SGameInstance::SGameInstance() : Super()
	, StartupLevel(SLevel::StaticClass())
{
}

void SGameInstance::Init()
{
}

std::wstring SGameInstance::GetApplicationName()
{
	return L"GameApp";
}

SLocalPlayer* SGameInstance::GetLocalPlayer()
{
	if (GEngine)
	{
		GEngine->GetSlateApplication()->GetLocalPlayer();
	}

	return nullptr;
}

SWorld* SGameInstance::GetWorld()
{
	return GEngine->GetEngineSubsystem<SGameLevelSystem>()->GetGameWorld();
}