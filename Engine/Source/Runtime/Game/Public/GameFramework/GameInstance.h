// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameInstance.generated.h"

SCLASS()
class GAME_API GameInstance : virtual public Object
{
	GENERATED_BODY()

	friend class GameApplication;

public:
	SCONSTRUCTOR()
	GameInstance();

protected:
	virtual void Init();
	virtual void Deinit();
};