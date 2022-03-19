// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class CoreDelegates
{
	CoreDelegates() = delete;

public:
	DECLARE_MULTICAST_DELEGATE(BeginMainInvokedDelegate);
	static BeginMainInvokedDelegate BeginMainInvoked;

	DECLARE_MULTICAST_DELEGATE(PostEngineInitDelegate);
	static PostEngineInitDelegate PostEngineInit;
};