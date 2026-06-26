// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <atomic>

namespace Ayla
{
	class LifetimeTestObject : public Object
	{
	private:
		static std::atomic<int32> s_ConstructedCount;
		static std::atomic<int32> s_DestroyedCount;

	public:
		LifetimeTestObject();
		virtual ~LifetimeTestObject() noexcept override;

		static void ResetCounters();
		static int32 GetConstructedCount();
		static int32 GetDestroyedCount();
		static ManagedTypeWrapper GetManagedType();

		static void GatherProperties(PropertyCollector& collection)
		{
			PLATFORM_UNREFERENCED_PARAMETER(collection);
		}
	};
}
