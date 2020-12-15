// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "IRHICommandList.h"

namespace SC::Runtime::Game::RHI
{
	interface IRHIImmediateCommandList : virtual public Core::Object, virtual public IRHICommandList
	{
		
	};
}