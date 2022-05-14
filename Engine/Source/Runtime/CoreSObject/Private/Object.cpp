// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Object.h"
#include "GC/GC.h"
#include "GC/WeakPtr.h"
#include "Reflection/Type.h"
#include "Reflection/TypeInfoMetadataGenerator.Impl.h"
#include "Reflection/MethodInfo.h"
#include "Reflection/MethodInfoMetadataGenerator.Impl.h"

using namespace libty;
using namespace libty::Reflection;

static std::tuple<> EmptyAttributes = std::make_tuple();
namespace libty::Generated::Assemblies
{
	extern SAssembly CoreSObject_AssemblyInfo;
}

SType SObject::StaticClass(libty::Core::Reflection::TypeInfoMetadataGenerator
	::GenerateManaged<"SObject"[0], SObject>(
		FriendlyName, TEXT("SObject"), &libty::Generated::Assemblies::CoreSObject_AssemblyInfo, EmptyAttributes
	)
);

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
	GC->Roots.emplace(this);
}

void SObject::RemoveFromRoot()
{
	GC->Roots.erase(this);
}

bool SObject::Equals(SObject* rhs)
{
	return this == rhs;
}

std::function<bool()> SObject::GetHolder()
{
	return [ptr = WeakPtr(this)]()
	{
		return ptr.IsValid();
	};
}

String SObject::ToString()
{
	return String(GetType()->GetFullQualifiedName());
}

void SObject::PostConstruction()
{
	GC->RegisterObject(this);

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