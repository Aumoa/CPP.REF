// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameModule.h"

class THSTORY_API STHGameModule : public SGameModule
{
	GENERATED_BODY(STHGameModule)

public:
	virtual SGameInstance* CreateGameInstance() override;
};