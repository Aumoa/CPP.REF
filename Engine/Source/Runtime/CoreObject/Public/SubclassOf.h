// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Type.h"
//#include "CoreAssert.h"

template<class T>
class SubclassOf
{
	template<class>
	friend class SubclassOf;

private:
	Type* _t;

public:
	inline SubclassOf()
		: SubclassOf(nullptr)
	{
	}

	inline SubclassOf(std::nullptr_t)
		: _t(nullptr)
	{
	}

	inline SubclassOf(Type* t)
		: _t(t)
	{
		check(t == nullptr || t->IsDerivedFrom(typeof(T)));
	}

	template<std::derived_from<T> U>
	inline SubclassOf(const SubclassOf<U>& derivedSubclass)
		: _t(derivedSubclass._t)
	{
	}

	inline Type* Get() const noexcept
	{
		return _t;
	}

	inline bool IsValid() const noexcept
	{
		return _t != nullptr;
	}

	inline Type* operator ->() const noexcept
	{
		return Get();
	}

	inline SubclassOf& operator =(const SubclassOf& rhs) noexcept
	{
		_t = rhs._t;
		check(_t == nullptr || _t->IsDerivedFrom(typeof(T)));
		return *this;
	}

	inline operator bool() const noexcept
	{
		return IsValid();
	}
};