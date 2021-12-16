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
	class ScopedTimer;

	template<class T>
	friend class GCRoot;
	friend class SObject;
	friend class Thread;

private:
	struct GCWorkerThread
	{
		Thread* ThreadPtr = nullptr;
		std::future<void> ThreadJoin;
		std::array<int64, 10240> DummyWorks;

		std::mutex WorkerMtx;
		std::mutex AtomicMtx;

		std::function<void()> Work;
	};

	static constexpr size_t NumGCWorkerThreads = 16;
	std::array<GCWorkerThread, NumGCWorkerThreads> GCWorkerThreads;
	std::atomic<bool> bRunningWorkers;
	std::condition_variable cvExecuteWorkers;

	void ReadyGCWorkers();
	void ShutdownGCWorkers();
	void ExecuteWorkers(bool bWait = true);

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

private:
	// lock-free buffers.
	static constexpr size_t NumGCThreads = 8;
	std::array<std::vector<SObject*>, NumGCThreads> ReferencedObjects_ThreadTemp;

private:
	GarbageCollector() = default;
	~GarbageCollector();

private:
	void RegisterObject(SObject* Object);
	void UnregisterObject(SObject* Object);

public:
	void Init();
	void Shutdown();

public:
	void Tick(float InDeltaSeconds);
	void Collect();
	void SuppressFinalize(SObject* Object);

	size_t NumThreadObjects();
	void SetAutoFlushInterval(float InSeconds);
	float GetAutoFlushInterval();

	void RegisterThread(Thread* ManagedThread);
	void UnregisterThread(Thread* ManagedThread);

private:
	bool IsMarked(SObject* Object);
	void MarkAndSweep(SObject* Object);
	void StopThreads(Thread* MyThread);
	void ResumeThreads(Thread* MyThread);
};