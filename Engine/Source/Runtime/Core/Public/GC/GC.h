// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include <set>
#include <list>
#include <functional>
#include <mutex>
#include <future>
#include <array>
#include <condition_variable>

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

enum class EGCThreadMode
{
	Manual,
	SpecifiedThreadHint,
	Auto,
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

public:
	struct ObjectPool
	{
		std::vector<SObject*> Collection;
		std::vector<size_t> IndexPool;
		size_t IndexPoolSize = 0;
		size_t NumPoolCompact = 0;

		size_t NumObjects();
		SObject*& Emplace(SObject* InObject);
		void Remove(SObject* InObject);

		void CompactIndexTable(size_t LiveIndex, std::vector<SObject*>& PendingKill);
	};

private:
	EGCLogVerbosity Verbosity = EGCLogVerbosity::Minimal;
	EGCThreadMode ThreadMode = EGCThreadMode::Auto;

	std::mutex GCMtx;

	ObjectPool Objects;
	std::set<SObject*> Roots;
	uint64 Generation = 0;
	std::set<SObject*> PendingFinalize;
	std::vector<SObject*> PendingKill;
	std::future<void> DeleteAction;

	std::set<Thread*> LogicThreads;

	float AutoFlushInterval = 60.0f;
	std::atomic<bool> bGCTriggered;
	std::atomic<bool> bLock;

	std::future<void> GCThread;
	std::atomic<bool> bRunningGCThread;

private:
	// lock-free buffers.
	static constexpr size_t NumGCThreads = 16;
	std::vector<std::future<void>> GCThreadFutures;
	std::vector<int32> GCMarkingBuffer;
	std::array<std::array<SObject*, 1024>, NumGCThreads> GCThreadBuffers = { };

private:
	GarbageCollector() = default;
	~GarbageCollector();

private:
	void RegisterObject(SObject* Object);
	void UnregisterObject(SObject* Object);

public:
	void Init();
	void Shutdown(bool bNormal);

	void RunAutoThread();
	void StopAutoThread();

public:
	void Tick(float InDeltaSeconds);
	void Collect(bool bFullPurge = false);
	void SuppressFinalize(SObject* Object);

	void SetThreadMode(EGCThreadMode ThreadMode);
	void Hint();
	void TriggerCollect();

	size_t NumObjects();
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