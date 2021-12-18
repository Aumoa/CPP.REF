// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngineSubsystem.h"

class PlatformModule;
class SGameInstance;
class SGameModule;

class GAME_API SGameModuleSystem : public SGameEngineSubsystem
{
	GENERATED_BODY(SGameModuleSystem)

private:
	std::unique_ptr<PlatformModule> Module;
	SPROPERTY(GameModule)
	SGameModule* GameModule = nullptr;

public:
	SGameModuleSystem();
	virtual ~SGameModuleSystem() override;

	virtual void Init() override;
	virtual void Deinit() override;
	
	void LoadGameModule(std::wstring_view GameModuleName);
	SGameInstance* LoadGameInstance();
};