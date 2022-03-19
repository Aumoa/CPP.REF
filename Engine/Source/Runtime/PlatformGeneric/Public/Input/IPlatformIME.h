// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IMEEvent.h"

struct IPlatformIME : implements SObject
{
	GENERATED_INTERFACE_BODY(IPlatformIME)

	DECLARE_MULTICAST_DELEGATE(IMEDelegate, IMEEvent);
	IMEDelegate IME;
};