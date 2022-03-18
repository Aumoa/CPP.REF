// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "ObjectHashTable.h"
#include <set>
#include <mutex>
#include <future>
#include <array>
#include "Misc/TickCalc.h"
#include "Delegates/MulticastEvent.h"

class SObject;
class GarbageCollector;
class Thread;

CORESOBJECT_API extern GarbageCollector& GC;

template<class T>
concept IsGCClass = requires
{
	{ std::remove_reference_t<std::remove_pointer_t<T>>::StaticClass() };
};

template<class TTupleClass>
struct NumGCTypes
{
	static constexpr size_t Value = 0;
};

template<template<class...> class TTupleClass, class TTupleType, class... TTupleTypes>
class NumGCTypes<TTupleClass<TTupleType, TTupleTypes...>>
{
	static consteval size_t GetImpl()
	{
		constexpr bool Test = IsGCClass<TTupleType>;
		if constexpr (sizeof...(TTupleTypes) == 0)
		{
			return Test ? 1 : 0;
		}
		else
		{
			return (Test ? 1 : 0) + NumGCTypes<TTupleClass<TTupleTypes...>>::GetImpl();
		}
	}

public:
	static constexpr size_t Value = GetImpl();
};

template<class T>
concept IsMutableCollection = requires (T & Collection)
{
	{ *Collection.begin() = {} };
};

class CORESOBJECT_API GarbageCollector
{
public:
	using This = GarbageCollector;
	class Instantiate;
	class ScopedTimer;

	template<class T>
	friend class GCRoot;
	friend class SObject;
	friend class Thread;
	friend class Type;

private:
	std::mutex GCMtx;

	ObjectHashTable Objects;
	std::set<SObject*> Roots;
	uint64 Generation = 0;
	std::set<SObject*> PendingFinalize;
	std::vector<SObject*> PendingKill;
	std::future<void> DeleteAction;

	std::future<void> GCThread;
	std::atomic<bool> bRunningGCThread;

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
	GarbageCollector() = default;
	~GarbageCollector();

private:
	void RegisterObject(SObject* Object);
	void UnregisterObject(SObject* Object);

public:
	void Init();
	void Shutdown(bool bNormal);

public:
	void Collect(bool bFullPurge = false);
	void SuppressFinalize(SObject* Object);

	void Hint();
	void TriggerCollect();

	size_t NumObjects();

	void SetFlushInterval(float InSeconds);
	float GetFlushInterval();

public:
	static GarbageCollector& Get();

public:
	DECLARE_MULTICAST_EVENT(GCEvent);
	GCEvent PreGarbageCollect;
	GCEvent PostGarbageCollect;

private:
	int32 MarkGC(SObject* Object, size_t ThreadIdx, int32 MarkDepth);
};