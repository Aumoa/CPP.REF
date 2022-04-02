// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GC/GC.h"
#include "Threading/Tasks/Parallel.h"
#include "Diagnostics/CycleCounterNamespace.h"
#include "Diagnostics/CycleCounterUnit.h"
#include "Diagnostics/CycleCounterMacros.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogVerbosity.h"
#include "Threading/SuspendTokenCollection.h"
#include "Threading/ISuspendToken.h"
#include "Reflection/Type.h"
#include "Reflection/FieldInfo.h"
#include "LogCore.h"
#include "Object.h"
#include <string>
#include <future>
#include <cstring>

DEFINE_LOG_CATEGORY(LogGC);

DECLARE_STAT_GROUP("GC", STATGROUP_GC);

DECLARE_CYCLE_STAT("GC", STAT_GC, STATGROUP_GC);
DECLARE_CYCLE_STAT("  Mark", STAT_Mark, STATGROUP_GC);
DECLARE_CYCLE_STAT("  Sweep", STAT_Sweep, STATGROUP_GC);
DECLARE_CYCLE_STAT("  CompactObjectTable", STAT_CompactObjectTable, STATGROUP_GC);
DECLARE_CYCLE_STAT("  Finalize", STAT_Finalize, STATGROUP_GC);
DECLARE_CYCLE_STAT("  PreCollect", STAT_PreCollect, STATGROUP_GC);
DECLARE_CYCLE_STAT("  PostCollect", STAT_PostCollect, STATGROUP_GC);
DECLARE_CYCLE_STAT("  SuspendToken", STAT_SuspendToken, STATGROUP_GC);
DECLARE_CYCLE_STAT("  ResumeToken", STAT_ResumeToken, STATGROUP_GC);

using namespace libty;

GarbageCollector& libty::Core::GC = GarbageCollector::Get();

GarbageCollector::~GarbageCollector()
{
	Shutdown(false);
}

void GarbageCollector::RegisterObject(SObject* Object)
{
	std::unique_lock GCMtx_lock(GCMtx);
	Objects.Emplace(Object);
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
	bTearingDown = true;

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

bool GarbageCollector::IsTearingDown()
{
	return bTearingDown;
}

void GarbageCollector::Collect(bool bFullPurge)
{
	SCOPE_CYCLE_COUNTER(STAT_GC);
	SE_LOG(LogGC, Verbose, L"GC.Collect() triggered.");

	{
		SCOPE_CYCLE_COUNTER(STAT_PreCollect);
		PreGarbageCollect.Broadcast();
	}

	{
		SCOPE_CYCLE_COUNTER(STAT_SuspendToken);
		std::vector<std::future<void>> SuspendReady;
		auto& SuspendTokens = SuspendTokenCollection::Collection();
		SuspendReady.reserve(SuspendTokens.size());

		for (auto& Token : SuspendTokens)
		{
			SuspendReady.emplace_back(Token->Suspend());
		}

		// When all tokens are ready.
		auto timeout = std::chrono::steady_clock::now() + 20ms;
		bool timedOut = false;
		for (auto& Ready : SuspendReady)
		{
			//std::future_status status = Ready.wait_until(timeout);
			//if (status == std::future_status::timeout)
			//{
			//	timedOut = true;
			//	break;
			//}
			Ready.wait();
		}

		if (timedOut)
		{
			SE_LOG(LogGC, Verbose, L"Timed out detected for waiting that all threads are suspend.");

			{
				SCOPE_CYCLE_COUNTER(STAT_ResumeToken);
				auto& SuspendTokens = SuspendTokenCollection::Collection();
				for (auto& Token : SuspendTokens)
				{
					Token->Resume();
				}
			}

			{
				SCOPE_CYCLE_COUNTER(STAT_PostCollect);
				PostGarbageCollect.Broadcast();
			}

			return;
		}
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

			if (!StaticRoots.has_value())
			{
				StaticRoots.emplace();
				for (auto& [Name, StaticClass] : SType::_staticCollection->FullQualifiedNameView)
				{
					for (auto& Field : StaticClass->GetFields())
					{
						if (Field->IsStatic() && !Field->GetFieldType()->IsValueType())
						{
							StaticRoots->emplace_back(Field);
						}
					}
				}
			}

			for (auto& RField : *StaticRoots)
			{
				if (SObject* Value = RField->GetValue(nullptr); Value)
				{
					GCMarkingBuffer[Value->InternalIndex] = 1;
				}
			}

			std::atomic<int32> NumObjects = (int32)Roots.size();
			for (int32 Depth = 1; NumObjects != 0; ++Depth)
			{
				NumObjects = 0;
				Parallel::ForEach(FullCollectionSize, [&](size_t ThreadIdx, size_t Start, size_t End)
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

		{
			SCOPE_CYCLE_COUNTER(STAT_ResumeToken);
			auto& SuspendTokens = SuspendTokenCollection::Collection();
			for (auto& Token : SuspendTokens)
			{
				Token->Resume();
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
			PendingKill.resize(Objects.NumObjects());

			// Sweeping.
			Parallel::ForEach(ActualCollectionSize, [&](size_t ThreadIdx, size_t StartIdx, size_t EndIdx)
			{
				size_t MaxLiveNumber = 0;

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
						}
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
			SCOPE_CYCLE_COUNTER(STAT_CompactObjectTable);

			size_t BeforeCompact = Objects.TableSize();
			Objects.CompactIndexTable(LastObjectIndex, PendingKill);
			size_t AfterCompact = Objects.TableSize();
		}
	}

	DeleteAction = std::async([this]()
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

	{
		SCOPE_CYCLE_COUNTER(STAT_PostCollect);
		PostGarbageCollect.Broadcast();
	}
}

void GarbageCollector::SuppressFinalize(SObject* Object)
{
	Object->bHasFinalizer = false;
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

		if (Object == nullptr || Object->bMarkAtGC)
		{
			continue;
		}

		Object->bMarkAtGC = true;

		// Do NOT declare it as std::function because, that interfere function inlining.
		auto Marker = [&](SObject* Member)
		{
			if (Member == nullptr)
			{
				return;
			}

			if (Writep < (int32)Buffer.size())
			{
				Buffer[Writep++] = Member;
			}
			else
			{
				GCMarkingBuffer[Member->InternalIndex] = MarkDepth;
			}
			++Count;

#if !SHIPPING
			Member->GC_ContainsOwner = Object;
#endif
		};

		libty::Core::Reflection::FieldInfoMetadataGenerator::NativeGCInvoke MarkerFnc = Marker;

		for (auto& Field : Object->GetType()->GetFields())
		{
			SType* FieldType = Field->GetFieldType();
			if (!FieldType->IsValueType())
			{
				SObject* Member = Field->GetValue(Object);
				Marker(Member);
			}
			else if (Field->_meta.Collection)
			{
				Field->_meta.Collection(&Field->_meta, Object, MarkerFnc);
			}
		}
	}

	return Count;
}