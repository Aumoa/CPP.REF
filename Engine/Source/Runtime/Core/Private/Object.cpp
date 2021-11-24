// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Object.h"
#include "LogCore.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogSystem.h"

SObject::SObject()
{
}

SObject::~SObject() noexcept
{
	CleanupSubobjects();
}

std::wstring SObject::ToString(std::wstring_view InFormatArgs)
{
	return Name;
}

SObject* SObject::GetOuter() const
{
	return Outer;
}

std::shared_ptr<SObject> SObject::SetOuter(SObject* InNewOuter)
{
	std::shared_ptr<SObject> Detached;
	if (Outer != InNewOuter)
	{
		Detached = shared_from_this();

		if (Outer != nullptr)
		{
			Outer->InternalDetachSubobject(Detached.get());
		}

		if (InNewOuter != nullptr)
		{
			InNewOuter->InternalAttachSubobject(Detached.get());
		}

		Outer = InNewOuter;
	}
	return Detached;
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

void SObject::AddReferenceObject(SObject* InObject)
{
	InternalAttachSubobject(InObject);
}

void SObject::RemoveReferenceObject(SObject* InObject)
{
	InternalDetachSubobject(InObject);
}

void SObject::DestroyObject(SObject* InObject)
{
	checkf(InObject->Outer == this, L"DestroyObject must be called with outer object.");
	InternalDetachSubobject(InObject);
}

void SObject::CleanupSubobjects()
{
	for (auto& Subobject : Subobjects)
	{
		if (Subobject && Subobject->Outer == this)
		{
			// Detach from outer.
			Subobject->Outer = nullptr;
		}
	}

	Subobjects.clear();
}

void* SObject::operator new(size_t AllocSize)
{
	return new uint8[AllocSize];
}

void SObject::operator delete(void* MemBlock)
{
	delete[] reinterpret_cast<uint8*>(MemBlock);
}

void SObject::InternalDetachSubobject(SObject* Subobject)
{
	if (auto It = Subobjects.find(Subobject->SharedFromThis()); It != Subobjects.end())
	{
		if ((*It)->Outer == this)
		{
			(*It)->Outer = nullptr;
		}

		Subobjects.erase(It);
		return;
	}

	SE_LOG(LogCore, Error, L"Request destroy subobject but target is not valid subobject. Outer have not this subobject.");
}

void SObject::InternalAttachSubobject(SObject* Subobject)
{
	Subobjects.emplace(Subobject->SharedFromThis());
}

void SObject::InternalAttachObjectName(SObject* InObject)
{
	InObject->Name = InObject->GetType()->GenerateUniqueName();
}