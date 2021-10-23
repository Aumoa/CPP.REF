// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Object.h"
#include "LogCore.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogSystem.h"

std::atomic<uint64> SObject::InternalObjectIndexGenerator = 0;

SObject::SObject() : Index(++InternalObjectIndexGenerator)
{
}

SObject::~SObject() noexcept
{
}

std::wstring SObject::ToString(std::wstring_view InFormatArgs)
{
	return GetType()->GetFriendlyName();
}

SObject* SObject::GetOuter() const
{
	return Outer;
}

std::shared_ptr<SObject> SObject::SetOuter(SObject* InNewOuter)
{
	std::shared_ptr<SObject> Detached = shared_from_this();

	if (Outer != nullptr)
	{
		Outer->InternalDetachSubobject(Detached.get());
	}

	if (InNewOuter != nullptr)
	{
		InNewOuter->InternalAttachSubobject(Detached.get());
	}

	Outer = InNewOuter;
	return Detached;
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
	SObject* outer = InObject->Outer;
	if (outer == nullptr)
	{
		delete InObject;
		return;
	}

	outer->InternalDetachSubobject(InObject);
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
	if (auto it = Views.find(Subobject); it != Views.end())
	{
		auto subobject_it = Subobjects.begin() + it->second;
		(*subobject_it)->Outer = nullptr;

		subobject_it->reset();
		Views.erase(it);
		return;
	}

	LogSystem::Log(LogCore, ELogVerbosity::Error, L"Request destroy subobject but target is not valid subobject. Outer have not this subobject.");
}

void SObject::InternalAttachSubobject(SObject* Subobject)
{
	for (size_t i = 0; i < Subobjects.size(); ++i)
	{
		if (!Subobjects[i])
		{
			Views.emplace(Subobject, i);
			Subobjects[i] = Subobject->shared_from_this();
			return;
		}
	}

	Views.emplace(Subobject, Subobjects.size());
	Subobjects.emplace_back(Subobject->shared_from_this());
}