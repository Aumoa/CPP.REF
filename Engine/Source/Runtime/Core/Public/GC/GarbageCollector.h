// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include <set>
#include <list>

class SObject;

class CORE_API GarbageCollector
{
	static constexpr size_t IncrementalDeleteObject = 1024;

	template<class T>
	friend class GCRoot;
	friend class SObject;

	std::set<SObject*> Collection;
	std::set<SObject*> Roots;
	uint64 Generation = 0;

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