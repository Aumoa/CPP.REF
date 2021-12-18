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
	GC.RegisterObject(this);
}

SObject::~SObject()
{
	if (ReferencePtr->WeakReferences == 0)
	{
		delete ReferencePtr;
		ReferencePtr = nullptr;
	}
}

void SObject::UnmarkGC()
{
	ReferencePtr->bMarkAtGC = false;
}

void SObject::AddToRoot()
{
	GC.Roots.emplace(this);
}

void SObject::MarkGC(uint64 Generation)
{
	if (this->Generation != Generation)
	{
		this->Generation = Generation;
	}
	this->ReferencePtr->bMarkAtGC = true;
}

void SObject::RemoveFromRoot()
{
	GC.Roots.erase(this);
}

std::wstring SObject::ToString()
{
	return GetType()->GetFriendlyName();
}

void SObject::PostConstruction()
{
	const auto& CachingDummy = GetType()->GetGCProperties();
}

void* SObject::operator new(size_t AllocSize)
{
	return ::operator new(AllocSize);
}

void SObject::operator delete(void* MemBlock)
{
	::operator delete(MemBlock);
}