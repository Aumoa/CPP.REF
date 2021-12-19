// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GC/GC.h"
#include "Misc/TickCalc.h"
#include "Threading/Thread.h"
#include "Threading/Parallel.h"
#include "Object.h"
#include <string>

namespace std
{
	[[nodiscard]] auto size(GarbageCollector::ObjectPool& Pool)
	{
		return Pool.NumObjects();
	}

	[[nodiscard]] auto begin(GarbageCollector::ObjectPool& Pool)
	{
		return Pool.Collection.begin();
	}

	[[nodiscard]] auto end(GarbageCollector::ObjectPool& Pool)
	{
		return Pool.Collection.end();
	}
}

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
	EGCLogVerbosity MinimalVerbosity;
	std::wstring Name;
	TickCalc<> Timer;

public:
	ScopedTimer(std::wstring_view Name, EGCLogVerbosity MinimalVerbosity = EGCLogVerbosity::Verbose)
		: Name(Name), MinimalVerbosity(MinimalVerbosity)
	{
		if (GC.Verbosity >= MinimalVerbosity)
		{
			Timer.DoCalc();
		}
	}

	~ScopedTimer() noexcept
	{
		if (GC.Verbosity >= MinimalVerbosity)
		{
			float Time = Timer.DoCalc().count();
			SE_LOG(LogGC, Verbose, L"{}: {} millisecond elapsed.", Name, Time * 1000.0f);
		}
	}
};

size_t GarbageCollector::ObjectPool::NumObjects()
{
	return Collection.size() - (IndexPoolSize - NumPoolCompact);
}

SObject*& GarbageCollector::ObjectPool::Emplace(SObject* InObject)
{
	check(InObject->InternalIndex == -1);

	if (IndexPoolSize)
	{
		size_t Index = IndexPool[--IndexPoolSize];
		SObject*& Ref = Collection[Index] = InObject;
		InObject->InternalIndex = Index;
		return Ref;
	}

	size_t Idx = Collection.size();
	SObject*& Ref = Collection.emplace_back(InObject);
	IndexPool.emplace_back();
	InObject->InternalIndex = Idx;
	return Ref;
}

void GarbageCollector::ObjectPool::Remove(SObject* InObject)
{
	if (InObject->InternalIndex != -1)
	{
		size_t Index = InObject->InternalIndex;
		Collection[InObject->InternalIndex] = nullptr;
		IndexPool[IndexPoolSize++] = Index;
		InObject->InternalIndex = -1;
	}
}

void GarbageCollector::ObjectPool::CompactIndexTable(size_t LiveIndex, std::vector<SObject*>& PendingKill)
{
	for (SObject*& Object : PendingKill)
	{
		IndexPool[IndexPoolSize++] = Object->InternalIndex;
	}

	NumPoolCompact += Collection.size() - (LiveIndex + 1);
	Collection.resize(LiveIndex + 1);

	if (Collection.size())
	{
		size_t BackIterator = Collection.size() - 1;
		while (IndexPoolSize && Collection[BackIterator] && BackIterator != -1)
		{
			size_t Index = IndexPool[--IndexPoolSize];
			if (Index >= Collection.size())
			{
				NumPoolCompact -= 1;
				continue;
			}

			Collection[BackIterator]->InternalIndex = Index;
			std::swap(Collection[Index], Collection[BackIterator--]);
		}

		Collection.resize(BackIterator + 1);
	}
}

GarbageCollector& GC = GarbageCollector::Instantiate::GC();

GarbageCollector::~GarbageCollector()
{
	Shutdown(false);
}

void GarbageCollector::RegisterObject(SObject* Object)
{
	std::unique_lock GCMtx_lock(GCMtx);
	Objects.Emplace(Object);
	Object->Generation = Generation;
}

void GarbageCollector::UnregisterObject(SObject* Object)
{
	std::unique_lock GCMtx_lock(GCMtx);
	Objects.Remove(Object);
}

void GarbageCollector::Init()
{
}

void GarbageCollector::Shutdown(bool bNormal)
{
	std::unique_lock GCMtx_lock(GCMtx);

	if (DeleteAction.valid())
	{
		DeleteAction.get();
	}

	// Final dispose.
	for (auto& Item : Objects.Collection)
	{
		if (Item)
		{
			// Memory leak detected!
			ensure(!bNormal);
			delete Item;
		}
	}

	Objects.Collection.clear();
	Objects.IndexPool.clear();
	Roots.clear();
}

void GarbageCollector::RunAutoThread()
{
	GC.bRunningGCThread = true;

	auto GCThread = Thread::NewThread<void>(L"[GC Thread]", [&]()
	{
		while (GC.bRunningGCThread)
		{
			using namespace std::literals;

			GC.Tick(1.0f);
			std::this_thread::sleep_for(1s);
		}
	});
}

void GarbageCollector::StopAutoThread()
{
	if (GCThread.valid())
	{
		GC.bRunningGCThread = false;
		GCThread.get();
	}
}

void GarbageCollector::Tick(float InDeltaSeconds)
{
	static bool bInitialized = (Init(), true);
	static float IncrementalTime = 0;

	if (ThreadMode == EGCThreadMode::Manual)
	{
		return;
	}

	IncrementalTime += InDeltaSeconds;
	if (IncrementalTime >= AutoFlushInterval)
	{
		if (ThreadMode == EGCThreadMode::Auto)
		{
			bGCTriggered = false;
			Collect();
		}
		else
		{
			bGCTriggered = true;
		}

		IncrementalTime -= AutoFlushInterval;
	}
}

void GarbageCollector::Collect(bool bFullPurge)
{
	if (bLock)
	{
		return;
	}

	Thread* MyThread = Thread::GetCurrentThread();
	std::unique_lock GCMtx_lock(GCMtx);

	if (bFullPurge)
	{
		++Generation;
	}

	ScopedTimer Timer(std::format(L"Start GC {}, with {} objects, with {} size table.", Generation, Objects.NumObjects(), Objects.Collection.size()), EGCLogVerbosity::Minimal);

	{
		ScopedTimer Timer(L"  Mark");

		// Ready buffers for lock-free.
		size_t FullCollectionSize = Objects.Collection.size();
		GCMarkingBuffer.resize(FullCollectionSize);
		memset(GCMarkingBuffer.data(), 0, FullCollectionSize * sizeof(int32));

		for (auto& Root : Roots)
		{
			GCMarkingBuffer[Root->InternalIndex] = 1;
		}
		GCThreadFutures.resize(NumGCThreads);

		StopThreads(MyThread);

		std::atomic<int32> NumObjects = (int32)Roots.size();
		int32 Depth;
		for (Depth = 1; NumObjects != 0; ++Depth)
		{
			NumObjects = 0;
			Parallel::ForEach(GCThreadFutures, FullCollectionSize, [&](size_t ThreadIdx, size_t Start, size_t End)
			{
				int32 Count = 0;

				for (size_t i = Start; i < End; ++i)
				{
					if (GCMarkingBuffer[i] == Depth)
					{
						SObject* Object = Objects.Collection[i];
						if (Object)
						{
							Count += MarkGC(Object, ThreadIdx, Depth + 1);
						}
					}
				}

				NumObjects += Count;
			}, NumGCThreads);
		}

		ResumeThreads(MyThread);
		
		if (Verbosity >= EGCLogVerbosity::VeryVerbose)
		{
			SE_LOG(LogGC, Verbose, L"  DEPTH: {}", Depth);
		}
	}

	if (DeleteAction.valid())
	{
		//ScopedTimer Timer(L"  Wait Delete");
		DeleteAction.get();
	}

	PendingFinalize.clear();
	size_t LastObjectIndex = 0;

	{
		ScopedTimer Timer(L"  Sweep");

		size_t ActualCollectionSize = Objects.Collection.size();

		// Ready pending kill buffer.
		std::atomic<size_t> ActualPendingKill = 0;
		std::vector<size_t> ObjectCompactIndex(NumGCThreads);
		PendingKill.resize(Objects.NumObjects());

		// Sweeping.
		Parallel::ForEach(ActualCollectionSize, [&](size_t ThreadIdx, size_t StartIdx, size_t EndIdx)
		{
			size_t MaxLiveNumber = 0;
			for (size_t ItemIdx = StartIdx; ItemIdx < EndIdx; ++ItemIdx)
			{
				SObject* Object = Objects.Collection[ItemIdx];
				if (Object && Object->Generation != Generation && !Roots.contains(Object))
				{
					PendingKill[ActualPendingKill++] = Object;
					Object->UnmarkGC();
					Objects.Collection[Object->InternalIndex] = nullptr;
				}
				else
				{
					MaxLiveNumber = ItemIdx;
				}
			}

			ObjectCompactIndex[ThreadIdx] = MaxLiveNumber;
		}, NumGCThreads);

		// Compact objects list.
		PendingKill.resize(ActualPendingKill);

		for (auto& Index : ObjectCompactIndex)
		{
			if (Index > LastObjectIndex)
			{
				LastObjectIndex = Index;
			}
		}
	}

	// Swap-compact one item.
	{
		ScopedTimer Timer(L"  Compact object table.");

		size_t BeforeCompact = Objects.Collection.size();
		Objects.CompactIndexTable(LastObjectIndex, PendingKill);
		size_t AfterCompact = Objects.Collection.size();

		if (Verbosity >= EGCLogVerbosity::VeryVerbose)
		{
			SE_LOG(LogGC, Verbose, L"  Compact GC objects table: {} -> {}, {} discounts.", BeforeCompact, AfterCompact, BeforeCompact - AfterCompact);
		}
	}

	if (Verbosity >= EGCLogVerbosity::VeryVerbose)
	{
		SE_LOG(LogGC, Verbose, L"{} objects is unreachable.", PendingKill.size());
	}

	DeleteAction = std::async([this]()
	{
		size_t NumRemoves = 0;

		for (auto& Garbage : PendingKill)
		{
			delete Garbage;
		}

		PendingKill.clear();
	});

	if (bFullPurge)
	{
		DeleteAction.get();
	}

	++Generation;
}

void GarbageCollector::SuppressFinalize(SObject* Object)
{
	std::unique_lock GCMtx_lock(GCMtx);

	PendingFinalize.emplace(Object);
}

void GarbageCollector::SetThreadMode(EGCThreadMode ThreadMode)
{
	this->ThreadMode = ThreadMode;
}

void GarbageCollector::Hint()
{
	bool bTrue = true;
	if (ThreadMode == EGCThreadMode::SpecifiedThreadHint && bGCTriggered.compare_exchange_strong(bTrue, false))
	{
		Collect();
	}
}

void GarbageCollector::TriggerCollect()
{
	bGCTriggered = true;
}

size_t GarbageCollector::NumObjects()
{
	return Objects.NumObjects();
}

void GarbageCollector::SetAutoFlushInterval(float InSeconds)
{
	AutoFlushInterval = InSeconds;
}

float GarbageCollector::GetAutoFlushInterval()
{
	return AutoFlushInterval;
}

void GarbageCollector::SetLogVerbosity(EGCLogVerbosity Verbosity)
{
	this->Verbosity = Verbosity;
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

void GarbageCollector::Lock()
{
	bLock = true;
}

void GarbageCollector::Unlock()
{
	bLock = false;
}

bool GarbageCollector::IsMarked(SObject* Object)
{
	return Object->Generation == Generation;
}

int32 GarbageCollector::MarkGC(SObject* Object, size_t ThreadIdx, int32 MarkDepth)
{
	int32 Writep = 0;
	auto& Buffer = GCThreadBuffers[ThreadIdx];
	Buffer[Writep++] = Object;

	// Mark generation.
	int32 Count = 0;
	int32 Reap = 0;
	while (Reap < Writep)
	{
		Object = Buffer[Reap++];

		if (IsMarked(Object))
		{
			continue;
		}

		Object->MarkGC(Generation);

		for (auto& GCProp : Object->GetType()->GetGCProperties())
		{
			Type* PropertyType = GCProp->GetMemberType();
			if (PropertyType->IsGCCollection())
			{
				Count += PropertyType->MarkCollectionObjects(Object, GCProp, MarkDepth);
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
					if (Member)
					{
						if (Writep < (int32)Buffer.size())
						{
							Buffer[Writep++] = Member;
						}
						else
						{
							GCMarkingBuffer[Member->InternalIndex] = MarkDepth;
						}
						++Count;
					}
				}
			}
		}
	}

	return Count;
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