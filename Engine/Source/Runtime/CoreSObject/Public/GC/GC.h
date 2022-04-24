// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "ObjectHashTable.h"
#include <set>
#include <mutex>
#include <future>
#include <array>

namespace libty::inline Core
{
	class SObject;

	namespace Reflection
	{
		class SFieldInfo;
	}

	class CORESOBJECT_API GarbageCollector
	{
	public:
		using This = GarbageCollector;
		class Instantiate;
		class ScopedTimer;

		friend class libty::Core::SObject;
		friend class libty::Core::Reflection::SFieldInfo;

	private:
		static std::optional<GarbageCollector> sInstance;
		std::mutex GCMtx;

		ObjectHashTable Objects;
		std::set<SObject*> Roots;
		uint64 Generation = 0;
		std::vector<SObject*> PendingKill;
		std::future<void> DeleteAction;
		std::optional<std::vector<Reflection::SFieldInfo*>> StaticRoots;

		std::future<void> GCThread;
		std::atomic<bool> bRunningGCThread;
		bool bTearingDown = false;

	private:
		// lock-free buffers.
		static constexpr size_t NumGCThreads = 16;
		std::vector<int32> GCMarkingBuffer;
		std::array<std::array<SObject*, 1024>, NumGCThreads> GCThreadBuffers = { };

		TickCalc<> GCTimer;
		float FlushInterval = 60.0f;
		float TimeElapsed = 0.0f;
		std::atomic<bool> bGCTrigger = false;

	private:
		void RegisterObject(SObject* Object);
		void UnregisterObject(SObject* Object);

	public:
		GarbageCollector() = default;
		~GarbageCollector() noexcept;

		void Init();
		void Shutdown(bool bNormal) noexcept;
		bool IsTearingDown();

	public:
		void Collect(bool bFullPurge = false);
		void SuppressFinalize(SObject* Object);

		void Hint();
		void TriggerCollect();

		size_t NumObjects();

		void SetFlushInterval(float InSeconds);
		float GetFlushInterval();

	public:
		DECLARE_MULTICAST_EVENT(GCEvent);
		GCEvent PreGarbageCollect;
		GCEvent PostGarbageCollect;

	private:
		int32 MarkGC(SObject* Object, size_t ThreadIdx, int32 MarkDepth);
	};

	CORESOBJECT_API extern GarbageCollector* GC;
}