// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Object.h"
#include "LogCore.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogSystem.h"

std::atomic<uint64> SObject::_InternalObjectIndexGenerator = 0;

SObject::SObject() : _InternalObjectIndex(++_InternalObjectIndexGenerator)
{
}

SObject::~SObject() noexcept
{
}

std::wstring SObject::ToString(std::wstring_view formatArgs)
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
		_outer->InternalDetachSubobject(detached.get());
	}

	if (newOuter != nullptr)
	{
		newOuter->InternalAttachSubobject(detached.get());
	}

	_outer = newOuter;
	return detached;
}

uint64 SObject::GetInternalIndex()
{
	return _InternalObjectIndex;
}

void SObject::DestroySubobject(SObject* subobject)
{
	SObject* outer = subobject->_outer;
	if (outer == nullptr)
	{
		delete subobject;
		return;
	}

	outer->InternalDetachSubobject(subobject);
}

void* SObject::operator new(size_t size)
{
	return new uint8[size];
}

void SObject::operator delete(void* block)
{
	delete[] reinterpret_cast<uint8*>(block);
}

void SObject::InternalDetachSubobject(SObject* subobject)
{
	if (auto it = _views.find(subobject); it != _views.end())
	{
		auto subobject_it = _subobjects.begin() + it->second;
		(*subobject_it)->_outer = nullptr;

		subobject_it->reset();
		_views.erase(it);
		return;
	}

	LogSystem::Log(LogCore, ELogVerbosity::Error, L"Request destroy subobject but target is not valid subobject. Outer have not this subobject.");
}

void SObject::InternalAttachSubobject(SObject* subobject)
{
	for (size_t i = 0; i < _subobjects.size(); ++i)
	{
		if (!_subobjects[i])
		{
			_views.emplace(subobject, i);
			_subobjects[i] = subobject->shared_from_this();
			return;
		}
	}

	_views.emplace(subobject, _subobjects.size());
	_subobjects.emplace_back(subobject->shared_from_this());
}