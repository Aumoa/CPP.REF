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

public:
	class StackRoot
	{
	public:
		template<class... TStackRootRef>
		StackRoot(TStackRootRef&... Roots)
			: NumRoots(sizeof...(TStackRootRef))
		{
			ToRootArray = [&]()
			{
				std::vector<SObject*> Objects = { dynamic_cast<SObject*>(Roots)... };
				return Objects;
			};

			GC.StackRoots.emplace(this);
		}

		~StackRoot()
		{
			GC.StackRoots.erase(this);
		}
		
		const size_t NumRoots;
		std::function<std::vector<SObject*>()> ToRootArray;
	};

private:
	std::mutex Locker;
	std::atomic<bool> bScoped;

	std::set<SObject*> Collection;
	std::set<SObject*> Roots;
	uint64 Generation = 0;

	std::set<StackRoot*> StackRoots;

	std::set<SObject*> PendingFinalize;
	std::vector<SObject*> PendingKill;

	std::future<void> DeleteAction;

	std::set<Thread*> GCThreads;
	std::atomic<size_t> IncrementalGCMemory;

private:
	GarbageCollector();

private:
	void RegisterObject(SObject* Object);
	void UnregisterObject(SObject* Object);

	void RegisterThread(Thread* ManagedThread);
	void UnregisterThread(Thread* ManagedThread);

	void IncrementAllocGCMemory(size_t Amount);

public:
	void Collect(bool bFullPurge = false);
	size_t NumThreadObjects();
	void SuppressFinalize(SObject* Object);

private:
	void MarkAndSweep(SObject* Object);
};

#define GCROOTS_CONCAT_INTERNAL(X1, X2) X1 ## X2
#define GCROOTS_CONCAT(X1, X2) GCROOTS_CONCAT_INTERNAL(X1, X2)
#define GCROOTS(...) GarbageCollector::StackRoot GCROOTS_CONCAT(__FRAME_STACKROOTS_, __LINE__)(__VA_ARGS__)