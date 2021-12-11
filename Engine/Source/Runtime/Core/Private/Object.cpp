// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Object.h"
#include "LogCore.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogSystem.h"
#include "Misc/TickCalc.h"
#include "Threading/Parallel.h"

GENERATE_BODY(SObject);

SObject::SObject() : Generation(0)
{
	WeakReferences = std::make_shared<bool>(true);
}

SObject::~SObject()
{
	*WeakReferences = false;
}

std::wstring SObject::ToString()
{
	return Name;
}

void SObject::SetName(std::wstring_view InNewName)
{
	if (Name != InNewName)
	{
		Name = InNewName;
	}
}

std::wstring SObject::GetName()
{
	return Name;
}

SObject* SObject::GetOuter()
{
	return Outer;
}

void SObject::PostConstruction()
{
	Name = GetType()->GenerateUniqueName();
	GC().Collection.emplace(this);
	Generation = GC().Generation;
}

void* SObject::operator new(size_t AllocSize)
{
	void* Block = ::operator new(AllocSize, std::nothrow);
	if (Block == nullptr)
	{
		GC().Collect();
	}
	return Block;
}

void SObject::operator delete(void* MemBlock)
{
	::operator delete(MemBlock);
}

SObject::GarbageCollector::GarbageCollector()
{
}

void SObject::GarbageCollector::Collect(bool bLog)
{
	TickCalc<> Timer;

	++Generation;
	[[unlikely]]
	if (bLog)
	{
		SE_LOG(LogGC, Verbose, L"Start collecting garbages with {} generation.", Generation);
		Timer.DoCalc();
	}

	if (Roots.size() > 4)
	{
		std::vector<SObject*> FlattenRoots;
		FlattenRoots.reserve(Roots.size());

		for (auto& Root : Roots)
		{
			FlattenRoots.emplace_back(Root);
		}

		Parallel::For(FlattenRoots.size(), [&](size_t Idx)
		{
			FlattenRoots[Idx]->MarkAndSweep(Generation);
		}, 4);
	}
	else
	{
		for (auto& Root : Roots)
		{
			Root->MarkAndSweep(Generation);
		}
	}

	static std::vector<std::set<SObject*>::iterator> Garbages;
	Garbages.reserve(Collection.size());

	for (auto It = Collection.begin(); It != Collection.end(); ++It)
	{
		if ((*It)->Generation != Generation)
		{
			Garbages.emplace_back(It);
		}
	}

	for (auto& Garbage : Garbages)
	{
		delete *Garbage;
		Collection.erase(Garbage);
	}

	[[unlikely]]
	if (bLog)
	{
		float ElapsedTime = Timer.DoCalc().count() * 1000.0f;
		SE_LOG(LogGC, Verbose, L"{} garbages are collected. (Total: {} objects, {} milliseconds elapsed.)", Garbages.size(), Collection.size(), ElapsedTime);
	}
	Garbages.clear();
}

size_t SObject::GarbageCollector::NumThreadObjects()
{
	return Collection.size();
}

void SObject::GarbageCollector::SuppressFinalize(SObject* Object)
{
	Collection.erase(Object);
}

void SObject::GarbageCollector::Consume(GarbageCollector& AnotherThreadGC)
{
	AnotherThreadGC.Collect();
	Collection.insert(AnotherThreadGC.Collection.begin(), AnotherThreadGC.Collection.end());
	Roots.insert(AnotherThreadGC.Roots.begin(), AnotherThreadGC.Roots.end());
}

auto SObject::GC() -> GarbageCollector&
{
	static thread_local GarbageCollector GC;
	return GC;
}

void SObject::MarkAndSweep(uint64 Generation)
{
	if (this->Generation == Generation)
	{
		return;
	}

	this->Generation = Generation;
	for (auto& GCProp : GetType()->GetGCProperties())
	{
		Type* PropertyType = GCProp->GetMemberType();
		if (PropertyType->IsGCCollection())
		{
			for (auto& CollectionMember : PropertyType->GetCollectionObjects(this, GCProp))
			{
				if (CollectionMember)
				{
					CollectionMember->MarkAndSweep(Generation);
				}
			}
		}
		else
		{
			SObject* Member = GCProp->GetObject(this);
			if (Member)
			{
				Member->MarkAndSweep(Generation);
			}
		}
	}
}