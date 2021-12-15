// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GC/GarbageCollector.h"
#include "Misc/TickCalc.h"
#include "Threading/Thread.h"
#include "Threading/Parallel.h"
#include "Object.h"
#include <string>

class GarbageCollector::Instantiate
{
public:
	static GarbageCollector& GC()
	{
		static GarbageCollector Instance;
		return Instance;
	}
};

GarbageCollector& GC = GarbageCollector::Instantiate::GC();

#define SCOPED_LOCK() \
std::unique_lock<std::mutex> Lock(Locker);

namespace
{
	class ScopedTimer
	{
		std::wstring Name;
		TickCalc<> Timer;

	public:
		ScopedTimer(std::wstring_view Name) : Name(Name)
		{
			Timer.DoCalc();
		}

		~ScopedTimer() noexcept
		{
			float Time = Timer.DoCalc().count();
			SE_LOG(LogGC, Verbose, L"{}: {} millisecond elapsed.", Name, Time * 1000.0f);
		}
	};
}

size_t GarbageCollector::ObjectPool::size()
{
	return Collection.size() - PoolReserve.size();
}

SObject*& GarbageCollector::ObjectPool::emplace(SObject* InObject)
{
	check(InObject->InternalIndex == -1);

	if (PoolReserve.size())
	{
		size_t Idx = PoolReserve.back();
		PoolReserve.erase(--PoolReserve.end());
		SObject*& Ref = Collection[Idx] = InObject;
		InObject->InternalIndex = Idx;
		return Ref;
	}
	else
	{
		size_t Idx = Collection.size();
		SObject*& Ref = Collection.emplace_back(InObject);
		InObject->InternalIndex = Idx;
		return Ref;
	}
}

void GarbageCollector::ObjectPool::erase(SObject* InObject)
{
	if (InObject->InternalIndex != -1)
	{
		Collection[InObject->InternalIndex] = nullptr;
		PoolReserve.emplace_back(InObject->InternalIndex);
		InObject->InternalIndex = -1;
	}
}

GarbageCollector::GarbageCollector()
{
}

void GarbageCollector::RegisterObject(SObject* Object)
{
	SCOPED_LOCK();
	Objects.emplace(Object);
	Object->Generation = Generation;
}

void GarbageCollector::UnregisterObject(SObject* Object)
{
	SCOPED_LOCK();
	Objects.erase(Object);
}

void GarbageCollector::IncrementAllocGCMemory(size_t Amount)
{
	if (IncrementalGCMemory.fetch_add(Amount) > 104857600)
	{
		SE_LOG(LogGC, Verbose, L"Incremental GC memory is over than 100MB. Execute GC.");
		bManualGCTriggered = true;
		IncrementalGCMemory = 0;
	}
}

void GarbageCollector::RegisterThread(Thread* ManagedThread)
{
	SCOPED_LOCK();
	GCThreads.emplace(ManagedThread);
}

void GarbageCollector::UnregisterThread(Thread* ManagedThread)
{
	SCOPED_LOCK();
	GCThreads.erase(ManagedThread);
}

void GarbageCollector::Tick(float InDeltaSeconds)
{
	static constexpr size_t FullPurgeInterval = 10;
	static float IncrementalTime = 0;

	bool bExpect = true;
	IncrementalTime += InDeltaSeconds;
	if (IncrementalTime >= AutoFlushInterval || bManualGCTriggered.compare_exchange_strong(bExpect, false))
	{
		bool bFullPurge = ++MinorGCCounter >= FullPurgeInterval;
		// force full purge.
		Collect(true);
		bManualGCTriggered = false;

		IncrementalTime -= AutoFlushInterval;
		MinorGCCounter %= FullPurgeInterval;
	}
}

void GarbageCollector::Collect(bool bFullPurge)
{
	SCOPED_LOCK();

	Thread* MyThread = Thread::GetCurrentThread();

	SE_LOG(LogGC, Verbose, L"Start {} GC {}, with {} objects.", bFullPurge ? L"full purge" : L"minor", Generation, Objects.size());
	ScopedTimer Timer(L"Total");

	{
		ScopedTimer Timer(L"  Mark");

		if (bFullPurge)
		{
			// Ready buffers for lock-free.
			size_t FullCollectionSize = Objects.Collection.size();
			ReferencedObjects_ThreadTemp[0].reserve(FullCollectionSize);
			ReferencedObjects_ThreadTemp[1].reserve(FullCollectionSize);
			ReferencedObjects_ThreadTemp[2].reserve(FullCollectionSize);

			StopThreads(MyThread);

			for (auto& Root : Roots)
			{
				if (Root)
				{
					MarkAndSweep(Root, bFullPurge, 0);
				}
			}

			ResumeThreads(MyThread);
		}
		else
		{
			size_t FullCollectionSize = Objects.Collection.size();
			std::vector<std::future<void>> Futures;
			
			for (auto& ThreadCollection : ReferencedObjects_ThreadTemp)
			{
				ThreadCollection.reserve(FullCollectionSize);
			}

			// Mark for each all objects with 1 depth.
			Parallel::ForEach(Futures, [&]() { StopThreads(MyThread); }, FullCollectionSize, [&](size_t ThreadIdx, size_t ItemIdx)
			{
				SObject* Object = Objects.Collection[ItemIdx];

				if (Object == nullptr)
				{
					return;
				}

				MarkAndSweep(Object, bFullPurge, ThreadIdx);
			}, NumGCThreads);

			ResumeThreads(MyThread);
		}
	}

	if (DeleteAction.valid())
	{
		//ScopedTimer Timer(L"  Wait Delete");
		DeleteAction.get();
	}

	PendingFinalize.clear();

	{
		ScopedTimer Timer(L"  Sweep");

		size_t ActualCollectionSize = Objects.Collection.size();

		// Ready pending kill buffer.
		std::atomic<size_t> ActualPendingKill = 0;
		PendingKill.resize(Objects.size());

		// Sweeping.
		Parallel::ForEach(ActualCollectionSize, [&](size_t ThreadIdx, size_t ItemIdx)
		{
			SObject* Object = Objects.Collection[ItemIdx];
			if (Object && Object->Generation != Generation && !Roots.contains(Object))
			{
				PendingKill[ActualPendingKill++] = Object;
			}
		}, NumGCThreads);

		// Compact pending kill list.
		PendingKill.resize(ActualPendingKill);

		// Unregister pending kill objects.
		for (auto& Object : PendingKill)
		{
			Object->UnmarkGC();
			Objects.erase(Object);
		}
	}

	SE_LOG(LogGC, Verbose, L"{} objects is unreachable.", PendingKill.size());

	DeleteAction = std::async([this, bFullPurge]()
	{
		size_t NumRemoves = 0;

		for (auto& Garbage : PendingKill)
		{
			delete Garbage;
		}

		PendingKill.clear();
	});

	++Generation;
}

size_t GarbageCollector::NumThreadObjects()
{
	return Objects.size();
}

void GarbageCollector::SuppressFinalize(SObject* Object)
{
	SCOPED_LOCK();

	PendingFinalize.emplace(Object);
}

void GarbageCollector::Shutdown()
{
	if (DeleteAction.valid())
	{
		DeleteAction.get();
	}

	// Final dispose.
	for (auto& Item : Objects.Collection)
	{
		if (Item)
		{
			ensure(false);
			delete Item;
		}
	}

	Objects.Collection.clear();
	Objects.PoolReserve.clear();
	Roots.clear();
}

void GarbageCollector::SetAutoFlushInterval(float InSeconds)
{
	AutoFlushInterval = InSeconds;
}

float GarbageCollector::GetAutoFlushInterval()
{
	return AutoFlushInterval;
}

void GarbageCollector::MarkAndSweep(SObject* Object, bool bFullPurge, size_t ThreadIdx)
{
	if (bFullPurge)
	{
		std::vector<SObject*>& ReferencedObjects = ReferencedObjects_ThreadTemp[0];
		std::vector<SObject*>& Current = ReferencedObjects_ThreadTemp[1];
		std::vector<SObject*>& CollectionMember = ReferencedObjects_ThreadTemp[2];

		ReferencedObjects.emplace_back(Object);

		while (ReferencedObjects.size())
		{
			Current.clear();
			std::swap(Current, ReferencedObjects);

			for (auto& Object : Current)
			{
				if (Object == nullptr || (Object->Generation == Generation && Object->ReferencePtr->bMarkAtGC))
				{
					continue;
				}

				// Mark generation.
				Object->MarkGC(Generation);

				for (auto& GCProp : Object->GetType()->GetGCProperties())
				{
					Type* PropertyType = GCProp->GetMemberType();
					if (PropertyType->IsGCCollection())
					{
						PropertyType->GetCollectionObjects(Object, GCProp, ReferencedObjects);
						// Member object of collection cannot be changed to nullptr from disposed object.
					}
					else
					{
						SObject* Member = GCProp->GetObject(Object);

						// If object is disposed,
						if (PendingFinalize.contains(Member))
						{
							// Change property to nullptr.
							GCProp->SetObject(Object, nullptr);
						}
						else
						{
							ReferencedObjects.emplace_back(Member);
						}
					}
				}
			}
		}
	}
	else
	{
		std::vector<SObject*>& ThreadCollection = ReferencedObjects_ThreadTemp[ThreadIdx];

		for (auto& GCProp : Object->GetType()->GetGCProperties())
		{
			Type* PropertyType = GCProp->GetMemberType();
			if (PropertyType->IsGCCollection())
			{
				PropertyType->GetCollectionObjects(Object, GCProp, ThreadCollection);
				for (auto& Referenced : ThreadCollection)
				{
					if (Referenced)
					{
						Referenced->MarkGC(Generation);
					}
				}

				// Member object of collection cannot be changed to nullptr from disposed object.
			}
			else
			{
				SObject* Member = GCProp->GetObject(Object);
				if (Member)
				{
					// If object is disposed,
					if (PendingFinalize.contains(Member))
					{
						// Change property to nullptr.
						GCProp->SetObject(Object, nullptr);
					}
					else
					{
						Member->MarkGC(Generation);
					}
				}
			}
		}
	}
}

void GarbageCollector::StopThreads(Thread* MyThread)
{
	for (auto& GCThread : GCThreads)
	{
		if (MyThread->GetThreadId() != GCThread->GetThreadId())
		{
			GCThread->SuspendThread();
		}
	}
}

void GarbageCollector::ResumeThreads(Thread* MyThread)
{
	for (auto& GCThread : GCThreads)
	{
		if (MyThread != GCThread)
		{
			GCThread->ResumeThread();
		}
	}
}