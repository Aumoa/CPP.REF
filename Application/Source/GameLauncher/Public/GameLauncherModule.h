// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameModule.h"

class GAMELAUNCHER_API SGameLauncherModule : public SGameModule
{
	GENERATED_BODY(SGameLauncherModule)

public:
	virtual SGameInstance* CreatePrimaryGameModule() override;
};