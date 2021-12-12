// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GC/GarbageCollector.h"
#include "Misc/TickCalc.h"
#include "Object.h"
#include <string>

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

void GarbageCollector::Collect(bool bFullPurge)
{
	static thread_local std::vector<std::set<SObject*>::iterator> Garbages;

	if (!bFullPurge && PendingKill.size() > IncrementalDeleteObject * 10)
	{
		bFullPurge = true;
	}

	[[unlikely]]
	if (bFullPurge)
	{
		++Generation;
	}

	SE_LOG(LogGC, Verbose, L"Start GC {}", Generation);
	ScopedTimer Timer(L"Total");

	{
		ScopedTimer Timer(L"  Mark");

		for (auto& Root : Roots)
		{
			MarkAndSweep(Root);
		}
	}

	Garbages.reserve(Collection.size());

	{
		ScopedTimer Timer(L"  Sweep");

		for (auto It = Collection.begin(); It != Collection.end(); ++It)
		{
			if ((*It)->Generation != Generation)
			{
				Garbages.emplace_back(It);
			}
		}
	}

	{
		ScopedTimer Timer(L"  Delete");

		size_t NumRemoves = 0;

		for (auto& Garbage : Garbages)
		{
			if (NumRemoves++ < IncrementalDeleteObject || bFullPurge)
			{
				delete *Garbage;
			}
			else
			{
				PendingKill.emplace_back(*Garbage);
			}

			Collection.erase(Garbage);
		}

		size_t SizeIncrementalRemains = std::max((int64)IncrementalDeleteObject - (int64)NumRemoves, 0LL);
		for (auto It = PendingKill.begin(); It != PendingKill.end() && (SizeIncrementalRemains-- > 0 || bFullPurge);)
		{
			delete *It;
			if (bFullPurge)
			{
				PendingKill.erase(It);
				It = PendingKill.begin();
			}
			else
			{
				++It;
			}
		}

		if (bFullPurge)
		{
			PendingKill.clear();
		}
	}

	Garbages.clear();
	PendingFinalize.clear();

	[[unlikely]]
	if (!bFullPurge)
	{
		++Generation;
	}
}

void GarbageCollector::RegisterObject(SObject* Object)
{
	Collection.emplace(Object);
	Object->Generation = Generation;
}

size_t GarbageCollector::NumThreadObjects()
{
	return Collection.size();
}

void GarbageCollector::SuppressFinalize(SObject* Object)
{
	PendingFinalize.emplace(Object);
}

void GarbageCollector::Consume(GarbageCollector& AnotherThreadGC)
{
	AnotherThreadGC.Collect(true);

	Collection.insert(AnotherThreadGC.Collection.begin(), AnotherThreadGC.Collection.end());
	Roots.insert(AnotherThreadGC.Roots.begin(), AnotherThreadGC.Roots.end());
}

void GarbageCollector::From(GarbageCollector& AnotherThreadGC)
{
	Roots.insert(AnotherThreadGC.Roots.begin(), AnotherThreadGC.Roots.end());
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
			if (Object == nullptr || Object->Generation == Generation)
			{
				continue;
			}

			// Mark generation.
			Object->Generation = Generation;

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
		}
	}
}