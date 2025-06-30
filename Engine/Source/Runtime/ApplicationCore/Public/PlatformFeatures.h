// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	class GenericApplication;

	class APPLICATIONCORE_API PlatformFeatures
	{
	protected:
		PlatformFeatures();

	public:
		virtual ~PlatformFeatures() noexcept;

		virtual bool HasGenericApplication() = 0;
		virtual std::shared_ptr<GenericApplication> CreateGenericApplication() = 0;
	};
}