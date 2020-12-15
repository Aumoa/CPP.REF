// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "IRHIBundle.h"

namespace SC::Runtime::Game::RHI
{
	interface IRHISwapChain;
	interface IRHIImmediateCommandList;
	interface IRHICommandFence;

	interface IRHIDeviceBundle : virtual public Core::Object, virtual public IRHIBundle
	{
		virtual Core::TRefPtr<IRHISwapChain> GetSwapChain() const = 0;
		virtual Core::TRefPtr<IRHIImmediateCommandList> GetImmediateCommandList() const = 0;

		virtual Core::TRefPtr<IRHICommandFence> CreateCommandFence() = 0;
	};
}