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

class GarbageCollector::ScopedTimer
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

GarbageCollector& GC = GarbageCollector::Instantiate::GC();

void GarbageCollector::ReadyGCWorkers()
{
	size_t Index = 0;
	bRunningWorkers = true;

	for (auto& GCWorkerThread : GCWorkerThreads)
	{
		GCWorkerThread.ThreadJoin = Thread::NewThread<void>(std::format(L"[GC Worker Thread {}]", Index++), [&GCWorkerThread]()
		{
			GCWorkerThread.ThreadPtr = Thread::GetCurrentThread();
			GC.UnregisterThread(GCWorkerThread.ThreadPtr);

			std::mutex Mtx;
			std::unique_lock Mtx_lock(Mtx);

			while (GC.bRunningWorkers)
			{
				GC.cvExecuteWorkers.wait(Mtx_lock, [&GCWorkerThread]() { return (bool)GCWorkerThread.Work || !GC.bRunningWorkers; });

				if (GCWorkerThread.Work)
				{
					GCWorkerThread.Work();
					GCWorkerThread.Work = nullptr;
				}

				GCWorkerThread.WorkCompleted.set_value();
				GCWorkerThread.WorkCompleted = std::promise<void>();
			}
		});
	}
}

void GarbageCollector::ShutdownGCWorkers()
{
	bRunningWorkers = false;
	ExecuteWorkers();

	for (auto& Worker : GCWorkerThreads)
	{
		Worker.ThreadJoin.get();
	}
}

void GarbageCollector::ExecuteWorkers()
{
	cvExecuteWorkers.notify_all();
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

void GarbageCollector::RegisterObject(SObject* Object)
{
	std::unique_lock GCMtx_lock(GCMtx);
	Objects.emplace(Object);
	Object->Generation = Generation;
}

void GarbageCollector::UnregisterObject(SObject* Object)
{
	std::unique_lock GCMtx_lock(GCMtx);
	Objects.erase(Object);
}

void GarbageCollector::Init()
{
	ReadyGCWorkers();
}

void GarbageCollector::Shutdown()
{
	ShutdownGCWorkers();

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
		Collect();
		bManualGCTriggered = false;

		IncrementalTime -= AutoFlushInterval;
		MinorGCCounter %= FullPurgeInterval;
	}
}

void GarbageCollector::Collect()
{
	std::unique_lock GCMtx_lock(GCMtx);

	Thread* MyThread = Thread::GetCurrentThread();

	SE_LOG(LogGC, Verbose, L"Start GC {}, with {} objects.", Generation, Objects.size());
	ScopedTimer Timer(L"Total");

	{
		ScopedTimer Timer(L"  Mark");

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
				MarkAndSweep(Root);
			}
		}

		ResumeThreads(MyThread);
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

	DeleteAction = std::async([this]()
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
	std::unique_lock GCMtx_lock(GCMtx);

	PendingFinalize.emplace(Object);
}

void GarbageCollector::SetAutoFlushInterval(float InSeconds)
{
	AutoFlushInterval = InSeconds;
}

float GarbageCollector::GetAutoFlushInterval()
{
	return AutoFlushInterval;
}

void GarbageCollector::RegisterThread(Thread* ManagedThread)
{
	std::unique_lock GCMtx_lock(GCMtx);
	LogicThreads.emplace(ManagedThread);
}

void GarbageCollector::UnregisterThread(Thread* ManagedThread)
{
	std::unique_lock GCMtx_lock(GCMtx);
	LogicThreads.erase(ManagedThread);
}

void GarbageCollector::MarkAndSweep(SObject* Object)
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

void GarbageCollector::StopThreads(Thread* MyThread)
{
	for (auto& LogicThread : LogicThreads)
	{
		if (MyThread->GetThreadId() != LogicThread->GetThreadId())
		{
			LogicThread->SuspendThread();
		}
	}
}

void GarbageCollector::ResumeThreads(Thread* MyThread)
{
	for (auto& LogicThread : LogicThreads)
	{
		if (MyThread != LogicThread)
		{
			LogicThread->ResumeThread();
		}
	}
}