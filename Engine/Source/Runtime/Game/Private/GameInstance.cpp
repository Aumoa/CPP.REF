// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GameInstance.h"
#include "GameEngine.h"
#include "Level/Level.h"
#include "EngineSubsystems/GameLevelSystem.h"
#include "EngineSubsystems/GameInstanceSubsystem.h"

DEFINE_LOG_CATEGORY(LogGameInstance);

using namespace ::libty;

SGameInstance::SGameInstance() : Super()
	, StartupLevel(SLevel::TypeId)
{
}

void SGameInstance::Init()
{
	InitSubsystemCollection();
}

SLocalPlayer* SGameInstance::GetLocalPlayer()
{
	if (GEngine)
	{
		//return GEngine->GetSlateApplication()->GetLocalPlayer();
	}

	return nullptr;
}

SWorld* SGameInstance::GetWorld()
{
	return GEngine->GetEngineSubsystem<SGameLevelSystem>()->GetGameWorld();
}

SGameInstanceSubsystem* SGameInstance::GetSubsystem(SType* SubsystemClass, bool bAllowDerivedClass)
{
	for (auto& Subsystem : Subsystems)
	{
		if (bAllowDerivedClass)
		{
			if (Subsystem->GetType()->IsDerivedFrom(SubsystemClass))
			{
				return Subsystem;
			}
		}
		else
		{
			if (Subsystem->GetType()->IsA(SubsystemClass))
			{
				return Subsystem;
			}
		}
	}

	SE_LOG(LogGameInstance, Error, L"Subsystem typed as '{}' is not found.", SubsystemClass->GetName());
	return nullptr;
}

void SGameInstance::InitSubsystemCollection()
{
	auto SubsystemClasses = SType::GetDerivedTypes(typeof(SGameInstanceSubsystem));
	SE_LOG(LogGameInstance, Verbose, L"{} subsystems found.", SubsystemClasses.size() - 1);

	for (auto& Class : SubsystemClasses)
	{
		if (Class->IsA<SGameInstanceSubsystem>())
		{
			continue;
		}

		auto Instance = Cast<SGameInstanceSubsystem>(Class->Instantiate());
		Instance->Init(this);
		Subsystems.emplace_back(Instance);
	}
}