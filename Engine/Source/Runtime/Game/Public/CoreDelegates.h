// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CoreDelegates.generated.h"

SCLASS()
class GAME_API CoreDelegates : virtual public Object
{
	GENERATED_BODY()

private:
	CoreDelegates() = delete;

public:
	DECLARE_MULTICAST_DELEGATE(BeginMainInvokedDelegate);
	static BeginMainInvokedDelegate BeginMainInvoked;

	DECLARE_MULTICAST_DELEGATE(PostEngineInitDelegate);
	static PostEngineInitDelegate PostEngineInit;
};