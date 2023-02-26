// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class LAUNCH_API NLaunch
{
private:
	String CmdArgs;

protected:
	NLaunch(String CmdArgs);

public:
	virtual ~NLaunch() noexcept;

public:
	int32 GuardedMain();
};