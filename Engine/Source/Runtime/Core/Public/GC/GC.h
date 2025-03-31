// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include "StaticClass.h"
#include "AssertionMacros.h"
#include "GC/RPtr.h"
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace Ayla
{
	class Object;
	template<class T>
	class RPtr;
	template<class T>
	class PPtr;

	struct CORE_API GC : public StaticClass
	{
		friend class Object;
		template<class T>
		friend class RPtr;
		template<class T>
		friend class PPtr;

	private:
		static std::chrono::seconds TimeSeconds;
		static int32 s_Interlocked;
		static int64 s_Version;
		static std::thread::id s_ThreadId;
		static int32 s_DuringFinalize;
		static std::mutex s_NotifyMtx;
		static std::condition_variable s_CompleteToFinalize;

	public:
		static void SuppressFinalize(const RPtr<Object>& target);
		static void Collect(int32 generation = 2);
		static void WaitForCompleteToFinalize();

	private:
		static void AddRef(Object* target);
		static void Release(Object* target);
		static void InternalCollect(int32 generation, bool nolock);
		static void DoFinalize(int32 generation, std::vector<Object*> finalizedObjects, double criticalSectionSecs, double markObjectsSecs, double unmarkPropertyObjectsSecs, double finalizeQueueSecs);
	};
}