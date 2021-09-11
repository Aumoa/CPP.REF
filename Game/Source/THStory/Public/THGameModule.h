// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "GameModule.h"

class THSTORY_API THGameModule : public GameModule
{
	GENERATED_BODY(THGameModule)

public:
	virtual GameInstance* CreatePrimaryGameModule() override;
};