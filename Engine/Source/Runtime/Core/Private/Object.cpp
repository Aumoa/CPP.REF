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
	GC().RegisterObject(this);
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