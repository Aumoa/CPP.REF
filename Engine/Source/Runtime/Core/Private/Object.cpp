// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Object.h"
#include "LogCore.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogSystem.h"

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

SObject::GarbageCollector::GarbageCollector()
{
}

void SObject::GarbageCollector::Collect(bool bLog)
{
	++Generation;
	[[unlikely]]
	if (bLog)
	{
		SE_LOG(LogGC, Verbose, L"Start collecting garbages with {} generation.", Generation);
	}

	for (auto& Root : Roots)
	{
		Root->MarkAndSweep(Generation);
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
		SE_LOG(LogGC, Verbose, L"{} garbages are collected.", Garbages.size());
	}
	Garbages.clear();
}

size_t SObject::GarbageCollector::NumThreadObjects()
{
	return Collection.size();
}