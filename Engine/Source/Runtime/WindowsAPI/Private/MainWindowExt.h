// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#include "CoreMinimal.h"
#include "GenericWindowExtension.h"
#include "IGenericWindowDestroyEventHandler.h"

namespace Ayla
{
	class MainWindowExt : public GenericWindowExtension, public IGenericWindowDestroyEventHandler
	{
	public:
		MainWindowExt();
		virtual ~MainWindowExt() noexcept override;

		virtual void OnDestroy() override;
	};
}

#endif