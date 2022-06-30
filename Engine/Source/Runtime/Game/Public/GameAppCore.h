// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/CommandLineBuilder.h"
#include "GameAppCore.generated.h"

SCLASS()
class GAME_API GameAppCore : virtual public Object
{
	GENERATED_BODY()

public:
	GameAppCore();

	int32 StartApplication(const CommandLineBuilder& args);

public:
	static int32 GuardedMain(SubclassOf<GameAppCore> applicationClass, const CommandLineBuilder& args);
};