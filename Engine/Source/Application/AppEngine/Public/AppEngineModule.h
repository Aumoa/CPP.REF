// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameModule.h"

class APPENGINE_API SAppEngineModule : public SGameModule
{
	GENERATED_BODY(SAppEngineModule)

public:
	SAppEngineModule();
	virtual ~SAppEngineModule() override;

	virtual SGameEngine* CreateGameEngine() override;
};