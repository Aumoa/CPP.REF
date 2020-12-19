// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game::RHI
{
	interface IRHIRenderTargetView;

	interface IRHICommandList : virtual public Core::Object
	{
		virtual void OMSetRenderTargets(size_t count, IRHIRenderTargetView* rtv[]) = 0;
	};
}