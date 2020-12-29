// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "IRHICommandList.h"

namespace SC::Runtime::Game::RHI
{
	interface IRHIDeferredCommandList;

	interface IRHIImmediateCommandList : virtual public Core::Object, virtual public IRHICommandList
	{
		virtual void ExecuteCommandList(IRHIDeferredCommandList* deferredCommandList) = 0;
		virtual void Flush() = 0;
	};
}

#ifdef __SC_GLOBAL_NAMESPACE__
using SC::Runtime::Game::RHI::IRHIImmediateCommandList;
#endif