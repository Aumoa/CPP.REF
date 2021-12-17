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

enum class EGCLogVerbosity
{
	None,
	Minimal,
	Verbose,
	VeryVerbose,
};

class CORE_API GarbageCollector
{
public:
	class Instantiate;
	class ScopedTimer;

	template<class T>
	friend class GCRoot;
	friend class SObject;
	friend class Thread;
	friend class Type;

private:
	struct ObjectPool
	{
		std::vector<SObject*> Collection;
		std::set<size_t> PoolReserve;

		size_t size();
		SObject*& emplace(SObject* InObject);
		void erase(SObject* InObject);

		[[nodiscard]] auto begin() { return Collection.begin(); }
		[[nodiscard]] auto end() { return Collection.end(); }
	};

	std::mutex GCMtx;

	ObjectPool Objects;
	std::set<SObject*> Roots;
	uint64 Generation = 0;
	std::set<SObject*> PendingFinalize;
	std::vector<SObject*> PendingKill;
	std::future<void> DeleteAction;

	std::set<Thread*> LogicThreads;

	float AutoFlushInterval = 60.0f;
	std::atomic<bool> bManualGCTriggered;
	std::atomic<bool> bLock;

private:
	// lock-free buffers.
	static constexpr size_t NumGCThreads = 14;
	std::vector<std::future<void>> GCThreadFutures;
	std::vector<int32> GCMarkingBuffer;
	std::array<std::array<SObject*, 1024>, NumGCThreads> GCThreadBuffers;
	EGCLogVerbosity Verbosity;

private:
	GarbageCollector() = default;
	~GarbageCollector();

private:
	void RegisterObject(SObject* Object);
	void UnregisterObject(SObject* Object);

public:
	void Init();
	void Shutdown(bool bNormal);

public:
	void Tick(float InDeltaSeconds);
	void Collect();
	void SuppressFinalize(SObject* Object);

	size_t NumThreadObjects();
	void SetAutoFlushInterval(float InSeconds);
	float GetAutoFlushInterval();
	void SetLogVerbosity(EGCLogVerbosity Verbosity);

	void RegisterThread(Thread* ManagedThread);
	void UnregisterThread(Thread* ManagedThread);

	void Lock();
	void Unlock();

private:
	bool IsMarked(SObject* Object);
	int32 MarkGC(SObject* Object, size_t ThreadIdx, int32 MarkDepth);
	void StopThreads(Thread* MyThread);
	void ResumeThreads(Thread* MyThread);
};