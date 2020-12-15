// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game::RHI
{
	interface IRHIImmediateCommandList;

	interface IRHICommandFence : virtual public Core::Object
	{
		virtual void BeginFence() = 0;
		virtual void EndFence(IRHIImmediateCommandList* immediateCommandList) = 0;

		virtual bool IsFenceCompleted() const = 0;
		virtual bool Wait() const = 0;
	};
}