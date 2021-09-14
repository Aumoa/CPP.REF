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
	if (_outer != nullptr)
	{
		_outer->InternalDetachSubobject(this);
	}

	// Destroy all subobjects.
	for (auto it : _subobjects)
	{
		it->_outer = nullptr;
		delete it;
	}
}

std::wstring SObject::ToString(std::wstring_view formatArgs) const
{
	return GetType().GetFriendlyName();
}

SObject* SObject::GetOuter() const
{
	return _outer;
}

void SObject::SetOuter(SObject* newOuter)
{
	if (_outer != nullptr)
	{
		_outer->InternalDetachSubobject(this);
	}

	if (newOuter != nullptr)
	{
		newOuter->InternalAttachSubobject(this);
	}

	_outer = newOuter;
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

void SObject::InternalDetachSubobject(SObject* subobject)
{
	if (auto it = _subobjects.find(subobject); it != _subobjects.end())
	{
		(*it)->_outer = nullptr;
		_subobjects.erase(it);
		return;
	}

	LogSystem::Log(LogCore, ELogVerbosity::Error, L"Request destroy subobject but target is not valid subobject. Outer have not this subobject.");
}

void SObject::InternalAttachSubobject(SObject* subobject)
{
	_subobjects.emplace(subobject);
}

void SObject::InternalDestroySubobject(SObject* subobject)
{
	if (auto it = _subobjects.find(subobject); it != _subobjects.end())
	{
		SObject* ptr = *it;
		ptr->_outer = nullptr;
		_subobjects.erase(it);

		// Will remove all subobjects on destructor of object.
		delete ptr;
		return;
	}

	LogSystem::Log(LogCore, ELogVerbosity::Error, L"Request destroy subobject but target is not valid subobject. Outer have not this subobject.");
}