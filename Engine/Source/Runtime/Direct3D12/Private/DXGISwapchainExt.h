// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12Common.h"
#include "GenericWindowSwapchainExtension.h"
#include "IGenericWindowResizeEventHandler.h"
#include "DXGISwapchainExt.gen.h"

namespace Ayla
{
	ACLASS()
	class DXGISwapchainExt : public GenericWindowSwapchainExtension, public IGenericWindowResizeEventHandler
	{
		GENERATED_BODY()
	};
}