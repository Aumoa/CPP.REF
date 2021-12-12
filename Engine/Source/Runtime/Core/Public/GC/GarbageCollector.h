// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include <set>
#include <list>
#include <functional>

class SObject;

class CORE_API GarbageCollector
{
	template<class T>
	friend class GCRoot;
	friend class SObject;

public:
	static constexpr size_t IncrementalDeleteObject = 1024;

	class StackRoot
	{
		GarbageCollector& GC;

	public:
		template<class... TStackRootRef>
		StackRoot(GarbageCollector& GC, TStackRootRef&... Roots)
			: GC(GC)
			, NumRoots(sizeof...(TStackRootRef))
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
	std::set<SObject*> Collection;
	std::set<SObject*> Roots;
	uint64 Generation = 0;

	std::set<StackRoot*> StackRoots;

	std::set<SObject*> PendingFinalize;
	std::list<SObject*> PendingKill;

private:
	GarbageCollector();

public:
	void Collect(bool bFullPurge = false);
	size_t NumThreadObjects();
	void RegisterObject(SObject* Object);
	void SuppressFinalize(SObject* Object);

	void Consume(GarbageCollector& AnotherThreadGC);
	void From(GarbageCollector& AnotherThreadGC);
	void MarkAndSweep(SObject* Object);
};

#define GCROOTS_CONCAT_INTERNAL(X1, X2) X1 ## X2
#define GCROOTS_CONCAT(X1, X2) GCROOTS_CONCAT_INTERNAL(X1, X2)
#define GCROOTS(...) GarbageCollector::StackRoot GCROOTS_CONCAT(__FRAME_STACKROOTS_, __LINE__)(GC(), __VA_ARGS__)