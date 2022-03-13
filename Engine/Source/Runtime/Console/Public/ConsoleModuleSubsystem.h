// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"

class CONSOLE_API SConsoleModuleSubsystem : implements SObject
{
	GENERATED_BODY(SConsoleModuleSubsystem)

public:
	SConsoleModuleSubsystem();

	virtual void Init();
	virtual void Deinit();
};