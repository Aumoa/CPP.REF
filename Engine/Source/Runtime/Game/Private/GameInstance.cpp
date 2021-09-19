// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "GameInstance.h"
#include "GameEngine.h"
#include "Level/Level.h"
#include "EngineSubsystems/GamePlayerSystem.h"

SGameInstance::SGameInstance() : Super()
	, StartupLevel(SLevel::StaticClass())
{
}

void SGameInstance::Init()
{
}

std::wstring SGameInstance::GetApplicationName() const
{
	return L"GameApp";
}

SLocalPlayer* SGameInstance::GetLocalPlayer() const
{
	if (GEngine)
	{
		if (auto system = GEngine->GetEngineSubsystem<SGamePlayerSystem>(); system)
		{
			return system->GetLocalPlayer();
		}
	}

	return nullptr;
}