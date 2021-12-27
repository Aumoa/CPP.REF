// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"
#include "Threading/Parallel.h"
#include <string>

DECLARE_STAT_GROUP("GC", STATGROUP_GC);

DECLARE_CYCLE_STAT("GC", STAT_GC, STATGROUP_GC);
DECLARE_CYCLE_STAT("  Mark", STAT_Mark, STATGROUP_GC);
DECLARE_CYCLE_STAT("  Sweep", STAT_Sweep, STATGROUP_GC);
DECLARE_CYCLE_STAT("  CompactObjectTable", STAT_CompactObjectTable, STATGROUP_GC);
DECLARE_CYCLE_STAT("  Finalize", STAT_Finalize, STATGROUP_GC);
DECLARE_CYCLE_STAT("  PreCollect", STAT_PreCollect, STATGROUP_GC);
DECLARE_CYCLE_STAT("  PostCollect", STAT_PostCollect, STATGROUP_GC);

GarbageCollector& GC = GarbageCollector::Get();

GarbageCollector::~GarbageCollector()
{
	Shutdown(false);
}

void GarbageCollector::RegisterObject(SObject* Object)
{
	std::unique_lock GCMtx_lock(GCMtx);
	Objects.Emplace(Object);
	AppendMemorySize += Object->GetType()->GetSizeOf();

	if (AppendMemorySize > CachedMemorySize / 3 && !bGCTrigger)
	{
		TriggerCollect();
	}
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
	for (auto& Item : Objects)
	{
		if (Item)
		{
			// Memory leak detected!
			ensure(!bNormal);
			delete Item;
		}
	}

	Objects.Clear();
	Roots.clear();
}

void GarbageCollector::Collect(bool bFullPurge)
{
	SCOPE_CYCLE_COUNTER(STAT_GC);

	{
		SCOPE_CYCLE_COUNTER(STAT_PreCollect);
		PreGarbageCollect.Broadcast();
	}

	{
		std::unique_lock GCMtx_lock(GCMtx);
		++Generation;

		{
			SCOPE_CYCLE_COUNTER(STAT_Mark);

			// Ready buffers for lock-free.
			size_t FullCollectionSize = Objects.TableSize();
			GCMarkingBuffer.resize(FullCollectionSize);
			memset(GCMarkingBuffer.data(), 0, FullCollectionSize * sizeof(int32));

			for (auto& Root : Roots)
			{
				GCMarkingBuffer[Root->InternalIndex] = 1;
			}
			GCThreadFutures.resize(NumGCThreads);

			std::atomic<int32> NumObjects = (int32)Roots.size();
			for (int32 Depth = 1; NumObjects != 0; ++Depth)
			{
				NumObjects = 0;
				Parallel::ForEach(GCThreadFutures, FullCollectionSize, [&](size_t ThreadIdx, size_t Start, size_t End)
				{
					int32 Count = 0;

					for (size_t i = Start; i < End; ++i)
					{
						if (GCMarkingBuffer[i] == Depth)
						{
							SObject* Object = Objects.Get(i);
							if (Object)
							{
								Count += MarkGC(Object, ThreadIdx, Depth + 1);
							}
						}
					}

					NumObjects += Count;
				}, NumGCThreads);
			}
		}

		if (DeleteAction.valid())
		{
			//ScopedTimer Timer(L"  Wait Delete");
			DeleteAction.get();
		}

		size_t LastObjectIndex = 0;

		{
			SCOPE_CYCLE_COUNTER(STAT_Sweep);

			size_t ActualCollectionSize = Objects.TableSize();

			// Ready pending kill buffer.
			std::atomic<size_t> ActualPendingKill = 0;
			std::vector<size_t> ObjectCompactIndex(NumGCThreads);
			std::atomic<size_t> TotalMemorySize;
			PendingKill.resize(Objects.NumObjects());

			// Sweeping.
			Parallel::ForEach(ActualCollectionSize, [&](size_t ThreadIdx, size_t StartIdx, size_t EndIdx)
			{
				size_t MaxLiveNumber = 0;
				size_t MemorySize = 0;

				for (size_t ItemIdx = StartIdx; ItemIdx < EndIdx; ++ItemIdx)
				{
					SObject* Object = Objects.Get(ItemIdx);
					if (Object)
					{
						if (!Object->bMarkAtGC && !Roots.contains(Object))
						{
							PendingKill[ActualPendingKill++] = Object;
							Object->ReferencePtr->bDisposed = true;
							Objects.Set(Object->InternalIndex, nullptr);
						}
						else
						{
							Object->bMarkAtGC = false;
							MaxLiveNumber = ItemIdx;
							MemorySize += Object->GetType()->GetSizeOf();
						}
					}
				}

				ObjectCompactIndex[ThreadIdx] = MaxLiveNumber;
				TotalMemorySize += MemorySize;
			}, NumGCThreads);

			// Compact objects list.
			PendingKill.resize(ActualPendingKill);
			CachedMemorySize = TotalMemorySize;
			AppendMemorySize = 0;

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
			SCOPE_CYCLE_COUNTER(STAT_CompactObjectTable);

			size_t BeforeCompact = Objects.TableSize();
			Objects.CompactIndexTable(LastObjectIndex, PendingKill);
			size_t AfterCompact = Objects.TableSize();
		}

		DeleteAction = std::async([this, Finalized = std::move(PendingFinalize)]()
		{
			SCOPE_CYCLE_COUNTER(STAT_Finalize);
			size_t NumRemoves = 0;

			for (auto& Garbage : PendingKill)
			{
				if (Garbage->bHasFinalizer)
				{
					Garbage->Dispose(false);
				}
				delete Garbage;
			}

			PendingKill.clear();
		});

		if (bFullPurge)
		{
			DeleteAction.get();
		}
	}

	{
		SCOPE_CYCLE_COUNTER(STAT_PostCollect);
		PostGarbageCollect.Broadcast();
	}
}

void GarbageCollector::SuppressFinalize(SObject* Object)
{
	std::unique_lock GCMtx_lock(GCMtx);
	Object->bHasFinalizer = false;
	PendingFinalize.emplace(Object);
}

void GarbageCollector::Hint()
{
	TimeElapsed += GCTimer.DoCalc().count();

	if (TimeElapsed >= FlushInterval || bGCTrigger)
	{
		Collect();
		TimeElapsed = 0;
		bGCTrigger = false;
	}
}

void GarbageCollector::TriggerCollect()
{
	bGCTrigger = true;
}

size_t GarbageCollector::NumObjects()
{
	return Objects.NumObjects();
}

size_t GarbageCollector::MemorySize()
{
	return CachedMemorySize + AppendMemorySize;
}

void GarbageCollector::SetFlushInterval(float InSeconds)
{
	FlushInterval = InSeconds;
}

float GarbageCollector::GetFlushInterval()
{
	return FlushInterval;
}

GarbageCollector& GarbageCollector::Get()
{
	static GarbageCollector Singleton;
	return Singleton;
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

		if (Object == nullptr || Object->bMarkAtGC || PendingFinalize.contains(Object))
		{
			continue;
		}

		Object->bMarkAtGC = true;

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

#if DO_CHECK
						Member->GC_ContainsOwner = Object;
#endif
					}
				}
			}
		}
	}

	return Count;
}