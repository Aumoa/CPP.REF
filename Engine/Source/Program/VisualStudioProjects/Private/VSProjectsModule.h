// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ConsoleModule.h"

class SVSProjectsModule : public SConsoleModule
{
	GENERATED_BODY(SVSProjectsModule)

public:
	virtual int32 Main(const SCommandLine& CommandArgs) override;
};