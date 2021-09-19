// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Object.h"
#include "LogCore.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogSystem.h"

SObject::SObject()
{
}

SObject::~SObject() noexcept
{
}

std::wstring SObject::ToString(std::wstring_view formatArgs) const
{
	return GetType()->GetFriendlyName();
}

SObject* SObject::GetOuter() const
{
	return _outer;
}

std::shared_ptr<SObject> SObject::SetOuter(SObject* newOuter)
{
	std::shared_ptr<SObject> detached = shared_from_this();

	if (_outer != nullptr)
	{
		detached = _outer->InternalDetachSubobject(std::move(detached));
	}

	if (newOuter != nullptr)
	{
		newOuter->InternalAttachSubobject(std::move(detached));
	}

	_outer = newOuter;
	return detached;
}

void SObject::DestroySubobject(SObject* subobject)
{
	SObject* outer = subobject->_outer;
	if (outer == nullptr)
	{
		delete subobject;
		return;
	}

	outer->InternalDestroySubobject(subobject);
}

std::shared_ptr<SObject> SObject::InternalDetachSubobject(std::shared_ptr<SObject> subobject)
{
	if (auto it = _subobjects.find(subobject); it != _subobjects.end())
	{
		(*it)->_outer = nullptr;
		_subobjects.erase(it);
		return std::move(subobject);
	}

	LogSystem::Log(LogCore, ELogVerbosity::Error, L"Request destroy subobject but target is not valid subobject. Outer have not this subobject.");
	return {};
}

void SObject::InternalAttachSubobject(std::shared_ptr<SObject> subobject)
{
	_subobjects.emplace(subobject);
}

void SObject::InternalDestroySubobject(SObject* subobject)
{
	if (auto it = _subobjects.find(subobject->shared_from_this()); it != _subobjects.end())
	{
		SObject* ptr = it->get();
		ptr->_outer = nullptr;
		_subobjects.erase(it);

		// Will remove all subobjects on destructor of object.
		return;
	}

	LogSystem::Log(LogCore, ELogVerbosity::Error, L"Request destroy subobject but target is not valid subobject. Outer have not this subobject.");
}