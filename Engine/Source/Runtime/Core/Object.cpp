// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Object.h"
#include "LogCore.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogSystem.h"

using namespace std;

using enum ELogVerbosity;

Object::Object()
{
}

Object::~Object() noexcept
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

wstring Object::ToString(wstring_view formatArgs) const
{
	return L"Object";
}

Object* Object::GetOuter() const
{
	return _outer;
}

void Object::SetOuter(Object* newOuter)
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

void Object::DestroySubobject(Object* subobject)
{
	Object* outer = subobject->_outer;
	if (outer == nullptr)
	{
		delete outer;
		return;
	}

	outer->InternalDestroySubobject(subobject);
}

void Object::InternalDetachSubobject(Object* subobject)
{
	if (auto it = _subobjects.find(subobject); it != _subobjects.end())
	{
		(*it)->_outer = nullptr;
		_subobjects.erase(it);
		return;
	}

	LogSystem::Log(LogCore, Error, L"Request destroy subobject but target is not valid subobject. Outer have not this subobject.");
}

void Object::InternalAttachSubobject(Object* subobject)
{
	_subobjects.emplace(subobject);
}

void Object::InternalDestroySubobject(Object* subobject)
{
	if (auto it = _subobjects.find(subobject); it != _subobjects.end())
	{
		Object* ptr = *it;
		ptr->_outer = nullptr;
		_subobjects.erase(it);

		// Will remove all subobjects on destructor of object.
		delete ptr;
		return;
	}

	LogSystem::Log(LogCore, Error, L"Request destroy subobject but target is not valid subobject. Outer have not this subobject.");
}