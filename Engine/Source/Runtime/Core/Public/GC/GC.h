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
		static void TrapInitialize();
		static void SuppressFinalize(const RPtr<Object>& target);
		static void AddRef(Object* target);
		static void Release(Object* target);

	private:
		static void InternalCollect();

		static inline void EnsureFinalizerThread()
		{
			checkf(s_DuringFinalize, TEXT("The delete was called from an external scope."));
		}
	};
}