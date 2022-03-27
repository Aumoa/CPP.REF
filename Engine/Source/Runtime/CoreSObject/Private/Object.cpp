// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Object.h"
#include "LogCore.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogSystem.h"
#include "Misc/TickCalc.h"
#include "Threading/Parallel.h"
#include "GC/GC.h"
#include "GC/WeakPtr.h"
#include "Reflection/Type.h"
#include "Reflection/TypeInfoMetadataGenerator.Impl.h"

static std::tuple<> EmptyAttributes = std::make_tuple();
SType SObject::StaticClass(libty::Core::Reflection::TypeInfoMetadataGenerator::GenerateClass<SObject>(
	FriendlyName, "SObject", EmptyAttributes
));

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
	return std::wstring(GetType()->GetFullQualifiedName());
}

void SObject::PostConstruction()
{
	GC.RegisterObject(this);

#if !SHIPPING
	CachedTypeName = GetType()->GetFullQualifiedName();
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