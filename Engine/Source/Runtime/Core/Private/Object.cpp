// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Object.h"
#include "LogCore.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogSystem.h"
#include "Misc/TickCalc.h"
#include "Threading/Parallel.h"
#include "GC/GarbageCollector.h"

GENERATE_BODY(SObject);

SObject::SObject() : Generation(0)
{
	ReferencePtr = new WeakReferencePtr();
}

SObject::~SObject()
{
	bool bExpect = false;
	if (ReferencePtr->bDisposed.compare_exchange_strong(bExpect, true))
	{
		GC().UnregisterObject(this);
	}

	if (ReferencePtr->WeakReferences == 0)
	{
		delete ReferencePtr;
		ReferencePtr = nullptr;
	}
}

void SObject::MarkGC(uint64 Generation)
{
	this->Generation = Generation;
	this->ReferencePtr->bMarkAtGC = true;
}

void SObject::UnmarkGC()
{
	ReferencePtr->bMarkAtGC = false;
	if (!ReferencePtr->IsValid())
	{
		delete this;
		return;
	}
}

void SObject::AddToRoot()
{
	GC().Roots.emplace(this);
}

void SObject::RemoveFromRoot()
{
	GC().Roots.erase(this);
}

std::wstring SObject::ToString()
{
	return GetType()->GetFriendlyName();
}

void SObject::PostConstruction()
{
	GC().RegisterObject(this);
}

std::vector<SObject*> SObject::GetGCMembers()
{
	return {};
}

void* SObject::operator new(size_t AllocSize)
{
	void* Block = ::operator new(AllocSize, std::nothrow);
	if (Block == nullptr)
	{
		// First GC without full purge.
		GC().Collect();
		Block = ::operator new(AllocSize, std::nothrow);

		if (Block == nullptr)
		{
			// Second GC with full purge.
			GC().Collect(true);
			Block = ::operator new(AllocSize);
		}
	}
	return Block;
}

void SObject::operator delete(void* MemBlock)
{
	::operator delete(MemBlock);
}

auto SObject::GC() -> GarbageCollector&
{
	return GarbageCollector::GC();
}