// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	class IGenericWindowDestroyEventHandler
	{
	protected:
		IGenericWindowDestroyEventHandler()
		{
		}

		virtual ~IGenericWindowDestroyEventHandler() noexcept
		{
		}

	public:
		virtual void OnDestroy() = 0;
	};
}