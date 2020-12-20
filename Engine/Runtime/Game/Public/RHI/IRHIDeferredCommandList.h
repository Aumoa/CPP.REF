// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "IRHICommandList.h"

namespace SC::Runtime::Game::RHI
{
	interface IRHIDeferredCommandList : virtual public Core::Object, virtual public IRHICommandList
	{
		virtual void BeginCommand() = 0;
		virtual void EndCommand() = 0;
	};
}