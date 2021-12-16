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

GarbageCollector& GC = GarbageCollector::Instantiate::GC();

void GarbageCollector::ReadyGCWorkers()
{
	size_t Index = 0;
	bRunningWorkers = true;

	for (auto& GCWorkerThread : GCWorkerThreads)
	{
		std::promise<void> Startline;
		std::future<void> Startline_future = Startline.get_future();

		GCWorkerThread.ThreadJoin = Thread::NewThread<void>(std::format(L"[GC Worker Thread {}]", Index++), [Worker = &GCWorkerThread, &Startline]()
		{
			Worker->ThreadPtr = Thread::GetCurrentThread();
			GC.UnregisterThread(Worker->ThreadPtr);

			std::unique_lock Mtx_lock(Worker->WorkerMtx);
			Startline.set_value();

			using namespace std::literals;
			std::this_thread::sleep_for(10ms);

			while (GC.bRunningWorkers)
			{
				{
					std::unique_lock Atomic_lock(Worker->AtomicMtx);
					GC.cvExecuteWorkers.wait(Mtx_lock);
				}

				if (Worker->Work)
				{
					Worker->Work();
					Worker->Work = nullptr;
				}
			}
		});

		Startline_future.get();
		GCWorkerThread.WorkerMtx.lock();
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

void GarbageCollector::ExecuteWorkers(bool bWait)
{
	cvExecuteWorkers.notify_all();

	if (bWait)
	{
		for (auto& Worker : GCWorkerThreads)
		{
			// Worker can running from wait condition variable.
			Worker.WorkerMtx.unlock();
			while (!Worker.AtomicMtx.try_lock())
			{
				cvExecuteWorkers.notify_all();
			}
		}

		for (auto& Worker : GCWorkerThreads)
		{
			// Worker can running from wait condition variable.
			Worker.AtomicMtx.unlock();
			Worker.WorkerMtx.lock();
		}
	}
}

GarbageCollector::~GarbageCollector()
{
	Shutdown();
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
	static bool bInitialized = (Init(), true);
	static float IncrementalTime = 0;

	IncrementalTime += InDeltaSeconds;
	if (IncrementalTime >= AutoFlushInterval || bManualGCTriggered)
	{
		bManualGCTriggered = false;
		Collect();

		IncrementalTime -= AutoFlushInterval;
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

	{
		StopThreads(MyThread);

		ScopedTimer Timer(L"  DummyWorks");

		int64 Sum = 0;
		for (int32 i = 0; i < 10; ++i)
		{
			for (auto& GCThread : GCWorkerThreads)
			{
				for (size_t i = 0; i < GCThread.DummyWorks.size(); ++i)
				{
					int64 Value = GCThread.DummyWorks[i] ^= i;
					Sum += Value;
				}
			}
		}

		ResumeThreads(MyThread);

		SE_LOG(LogGC, Verbose, L"  DummyWorksResults: {}", Sum);
	}

	{
		ScopedTimer Timer(L"  DummyWorksMT");

		std::atomic<int64> Sum = 0;

		StopThreads(MyThread);

		for (int32 i = 0; i < 10; ++i)
		{
			for (auto& GCThread : GCWorkerThreads)
			{
				GCThread.Work = [Worker = &GCThread, &Sum]()
				{
					int64 LocalSum = 0;

					for (size_t i = 0; i < Worker->DummyWorks.size(); ++i)
					{
						int64 Value = Worker->DummyWorks[i] ^= i;
						LocalSum += Value;
					}

					Sum += LocalSum;
				};
			}

			ExecuteWorkers();
		}

		ResumeThreads(MyThread);

		SE_LOG(LogGC, Verbose, L"  DummyWorksMTResults: {}", Sum.load());
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

void GarbageCollector::SuppressFinalize(SObject* Object)
{
	std::unique_lock GCMtx_lock(GCMtx);

	PendingFinalize.emplace(Object);
}

size_t GarbageCollector::NumThreadObjects()
{
	return Objects.size();
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

bool GarbageCollector::IsMarked(SObject* Object)
{
	return Object == nullptr || (Object->Generation == Generation && Object->ReferencePtr->bMarkAtGC);
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
			if (IsMarked(Object))
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
						if (!IsMarked(Member))
						{
							ReferencedObjects.emplace_back(Member);
						}
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