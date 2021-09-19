// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngine.h"

class APPENGINE_API SAppEngine : public SGameEngine
{
	GENERATED_BODY(SAppEngine)

public:
	SAppEngine();
	virtual ~SAppEngine() override;

	virtual bool InitEngine() override;
	virtual void SetupFrameworkView(IFrameworkView* frameworkView) override;
};