// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include <set>
#include <list>
#include <functional>
#include <mutex>
#include <future>

class SObject;
class GarbageCollector;
class Thread;

CORE_API extern GarbageCollector& GC;

class CORE_API GarbageCollector
{
public:
	class Instantiate;

	template<class T>
	friend class GCRoot;
	friend class SObject;
	friend class Instantiate;
	friend class Thread;

private:
	struct ObjectPool
	{
		std::vector<SObject*> Collection;
		std::vector<size_t> PoolReserve;

		size_t size();
		SObject*& emplace(SObject* InObject);
		void erase(SObject* InObject);

		[[nodiscard]] auto begin() { return Collection.begin(); }
		[[nodiscard]] auto end() { return Collection.end(); }
	};

private:
	std::mutex Locker;
	std::atomic<bool> bScoped;

	ObjectPool Objects;
	std::set<SObject*> Roots;
	uint64 Generation = 0;

	std::set<SObject*> PendingFinalize;
	std::vector<SObject*> PendingKill;

	std::future<void> DeleteAction;

	std::set<Thread*> GCThreads;
	std::atomic<size_t> IncrementalGCMemory;

	float AutoFlushInterval = 60.0f;
	int32 MinorGCCounter = 0;
	std::atomic<bool> bManualGCTriggered;

private:
	// lock-free buffers.
	static constexpr size_t NumGCThreads = 8;
	std::array<std::vector<SObject*>, NumGCThreads> ReferencedObjects_ThreadTemp;

private:
	GarbageCollector();

private:
	void RegisterObject(SObject* Object);
	void UnregisterObject(SObject* Object);

	void IncrementAllocGCMemory(size_t Amount);

public:
	void RegisterThread(Thread* ManagedThread);
	void UnregisterThread(Thread* ManagedThread);

public:
	void Tick(float InDeltaSeconds);
	void Collect();
	size_t NumThreadObjects();
	void SuppressFinalize(SObject* Object);
	void Shutdown();

	void SetAutoFlushInterval(float InSeconds);
	float GetAutoFlushInterval();

private:
	void MarkAndSweep(SObject* Object);
	void StopThreads(Thread* MyThread);
	void ResumeThreads(Thread* MyThread);
};