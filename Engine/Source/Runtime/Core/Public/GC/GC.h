// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include "StaticClass.h"
#include "AssertionMacros.h"
#include "GC/RPtr.h"
#include <chrono>
#include <thread>

namespace Ayla
{
	class Object;

	struct CORE_API GC : public StaticClass
	{
		friend class Object;

	private:
		static std::chrono::seconds TimeSeconds;
		static int32 s_Interlocked;
		static int64 s_Version;
		static std::thread::id s_ThreadId;
		static bool s_DuringFinalize;

	public:
		static void SuppressFinalize(const RPtr<Object>& target);
		static void AddRef(Object* target);
		static void Release(Object* target);
		static void Collect(int32 generation = 2);

	private:
		static void InternalCollect(int32 generation, bool nolock);
		static void DoFinalize(int32 generation, std::vector<Object*> finalizedObjects, double criticalSectionSecs, double markObjectsSecs, double unmarkPropertyObjectsSecs, double finalizeQueueSecs);

		static inline void EnsureFinalizerThread()
		{
			checkf(s_DuringFinalize, TEXT("The delete was called from an external scope."));
		}
	};
}