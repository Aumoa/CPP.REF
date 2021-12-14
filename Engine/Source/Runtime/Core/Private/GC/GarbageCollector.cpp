// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GC/GarbageCollector.h"
#include "Misc/TickCalc.h"
#include "Threading/Thread.h"
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

namespace
{
	class ScopedGuard : public NonCopyable
	{
		std::atomic<bool>* Ptr = nullptr;

	public:
		ScopedGuard() = default;
		ScopedGuard(std::atomic<bool>* Ptr) : Ptr(Ptr)
		{
		}
		ScopedGuard(ScopedGuard&& Rhs) : Ptr(Rhs.Ptr)
		{
			Rhs.Ptr = nullptr;
		}
		~ScopedGuard() noexcept
		{
			if (Ptr)
			{
				*Ptr = false;
			}
		}

		ScopedGuard& operator =(ScopedGuard&& Rhs)
		{
			Ptr = Rhs.Ptr;
			Rhs.Ptr = nullptr;
			return *this;
		}
	};
}

#define SCOPED_LOCK() \
std::unique_lock<std::mutex> Lock; \
ScopedGuard Guard; \
if (bool bExpect = false; bScoped.compare_exchange_strong(bExpect, true)) \
{ \
	Lock = std::unique_lock(Locker); \
	Guard = ScopedGuard(&bScoped); \
}

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

GarbageCollector::GarbageCollector()
{
}

void GarbageCollector::RegisterObject(SObject* Object)
{
	SCOPED_LOCK();
	Collection.emplace(Object);
	Object->Generation = Generation;
}

void GarbageCollector::UnregisterObject(SObject* Object)
{
	SCOPED_LOCK();
	Collection.erase(Object);
}

void GarbageCollector::RegisterThread(Thread* ManagedThread)
{
	SCOPED_LOCK();
	check(ManagedThread->IsManaged());
	GCThreads.emplace(ManagedThread);
}

void GarbageCollector::UnregisterThread(Thread* ManagedThread)
{
	SCOPED_LOCK();
	GCThreads.erase(ManagedThread);
}

void GarbageCollector::IncrementAllocGCMemory(size_t Amount)
{
	if (IncrementalGCMemory.fetch_add(Amount) > 104857600)
	{
		SE_LOG(LogGC, Verbose, L"Incremental GC memory is over than 100MB. Execute GC.");
		Collect();
	}
}

void GarbageCollector::Collect(bool bFullPurge)
{
	SCOPED_LOCK();

	static std::vector<std::set<SObject*>::iterator> Garbages;
	Thread* MyThread = Thread::GetCurrentThread();

	[[unlikely]]
	if (bFullPurge)
	{
		++Generation;
	}

	SE_LOG(LogGC, Verbose, L"Start GC {}, with {} objects.", Generation, Collection.size());
	ScopedTimer Timer(L"Total");

	{
		//ScopedTimer Timer(L"  StopTheWorld");

		for (auto& GCThread : GCThreads)
		{
			if (MyThread != GCThread)
			{
				GCThread->SuspendThread();
			}
		}
	}

	{
		ScopedTimer Timer(L"  Mark");

		size_t NumRoots = Roots.size();
		for (auto& StackRoot : StackRoots)
		{
			NumRoots += StackRoot->NumRoots;
		}

		std::vector<SObject*> ActualRoots;
		ActualRoots.reserve(NumRoots);

		ActualRoots.insert(ActualRoots.end(), Roots.begin(), Roots.end());
		for (auto& StackRoot : StackRoots)
		{
			auto Array = StackRoot->ToRootArray();
			ActualRoots.insert(ActualRoots.end(), Array.begin(), Array.end());
		}

		for (auto& Root : ActualRoots)
		{
			if (Root)
			{
				MarkAndSweep(Root);
			}
		}
	}

	{
		//ScopedTimer Timer(L"  ResumeThreads");
		for (auto& GCThread : GCThreads)
		{
			if (MyThread != GCThread)
			{
				GCThread->ResumeThread();
			}
		}
	}

	if (DeleteAction.valid())
	{
		//ScopedTimer Timer(L"  Wait Delete");
		DeleteAction.get();
	}

	IncrementalGCMemory = 0;
	PendingFinalize.clear();

	Garbages.reserve(Collection.size());
	PendingKill.reserve(Collection.size());

	{
		ScopedTimer Timer(L"  Sweep");

		for (auto It = Collection.begin(); It != Collection.end(); ++It)
		{
			if ((*It)->Generation != Generation)
			{
				Garbages.emplace_back(It);
				PendingKill.emplace_back(*It);
			}
		}

		for (auto It : Garbages)
		{
			Collection.erase(It);
		}

		Garbages.clear();
	}

	DeleteAction = std::async([this, bFullPurge]()
	{
		size_t NumRemoves = 0;

		for (auto& Garbage : PendingKill)
		{
			Garbage->UnmarkGC();
		}

		PendingKill.clear();
	});

	[[unlikely]]
	if (!bFullPurge)
	{
		++Generation;
	}
}

size_t GarbageCollector::NumThreadObjects()
{
	return Collection.size();
}

void GarbageCollector::SuppressFinalize(SObject* Object)
{
	SCOPED_LOCK();

	PendingFinalize.emplace(Object);
}

void GarbageCollector::MarkAndSweep(SObject* Object)
{
	static thread_local std::vector<SObject*> ReferencedObjects;
	ReferencedObjects.emplace_back(Object);
	
	while (ReferencedObjects.size())
	{
		static thread_local std::vector<SObject*> Current;

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
					auto CollectionMember = PropertyType->GetCollectionObjects(Object, GCProp);
					ReferencedObjects.insert(ReferencedObjects.end(), CollectionMember.begin(), CollectionMember.end());

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

			{
				auto CollectionMember = Object->GetGCMembers();
				ReferencedObjects.insert(ReferencedObjects.end(), CollectionMember.begin(), CollectionMember.end());
			}
		}
	}
}