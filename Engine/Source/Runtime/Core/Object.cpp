// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.core;
import SC.Runtime.Core;

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
	for (size_t i = 0; i < _subobjects.size(); ++i)
	{
		delete _subobjects[i];
		_subobjects[i] = nullptr;
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
		LogSystem::Log(LogCore, Error, L"Request destroy subobject but target is not valid subobject. Outer is nullptr.");
		return;
	}

	outer->InternalDestroySubobject(subobject);
}

void Object::InternalDetachSubobject(Object* subobject)
{
	for (size_t i = 0; i < _subobjects.size(); ++i)
	{
		if (_subobjects[i] == subobject)
		{
			_subobjects[i]->_outer = nullptr;

			// RemoveAtSwap
			std::swap(_subobjects[_subobjects.size() - 1], _subobjects[i]);
			_subobjects.resize(_subobjects.size() - 1);
			return;
		}
	}

	LogSystem::Log(LogCore, Error, L"Request destroy subobject but target is not valid subobject. Outer have not this subobject.");
}

void Object::InternalAttachSubobject(Object* subobject)
{
	_subobjects.emplace_back(subobject);
}

void Object::InternalDestroySubobject(Object* subobject)
{
	for (size_t i = 0; i < _subobjects.size(); ++i)
	{
		if (_subobjects[i] == subobject)
		{
			Object* ptr = _subobjects[i];
			InternalDetachSubobject(subobject);

			// Will remove all subobjects on destructor of object.
			delete _subobjects[i];
			return;
		}
	}

	LogSystem::Log(LogCore, Error, L"Request destroy subobject but target is not valid subobject. Outer have not this subobject.");
}