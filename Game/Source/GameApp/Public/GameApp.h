// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ConsoleApplication.h"
#include "GameApp.generated.h"

SCLASS()
class GameApp : public ConsoleApplication
{
	GENERATED_BODY()

public:
	SCONSTRUCTOR()
	GameApp();

	SFUNCTION()
	int32 Run();
};