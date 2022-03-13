// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Object.h"
#include "LogCore.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogSystem.h"
#include "Misc/TickCalc.h"
#include "Threading/Parallel.h"
#include "GC/GC.h"
#include "GC/WeakPtr.h"

GENERATE_BODY(SObject);

SObject::SObject()
	: bMarkAtGC(false)
	, ReferencePtr(new Referencer())
{
	++ReferencePtr->WeakReferences;
}

SObject::SObject(SObject&& Rhs) noexcept
	: bMarkAtGC(false)
	, ReferencePtr(new Referencer())
{
	++ReferencePtr->WeakReferences;
}

SObject::~SObject()
{
	if (ReferencePtr && --ReferencePtr->WeakReferences == 0)
	{
		delete ReferencePtr;
		ReferencePtr = nullptr;
	}
}

void SObject::AddToRoot()
{
	GC.Roots.emplace(this);
}

void SObject::RemoveFromRoot()
{
	GC.Roots.erase(this);
}

std::function<bool()> SObject::GetHolder()
{
	return [ptr = WeakPtr(this)]()
	{
		return ptr.IsValid();
	};
}

std::wstring SObject::ToString()
{
	return GetType()->GetFriendlyName();
}

void SObject::PostConstruction()
{
	volatile const auto& CachingDummy = GetType()->GetGCProperties();
	GC.RegisterObject(this);

#if !SHIPPING
	CachedTypeName = GetType()->GetFullName();
#endif
}

void SObject::Dispose(bool bDisposing)
{
}

void* SObject::operator new(size_t AllocSize)
{
	return ::operator new(AllocSize);
}

void SObject::operator delete(void* MemBlock)
{
	::operator delete(MemBlock);
}