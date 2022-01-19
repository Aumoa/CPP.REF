// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Type.h"

template<class T>
class SubclassOf
{
	template<class>
	friend class SubclassOf;

private:
	Type* MyType = nullptr;

public:
	inline SubclassOf() = default;

	inline SubclassOf(const SubclassOf& Rhs) : MyType(Rhs.MyType)
	{
	}

	inline SubclassOf(SubclassOf&& Rhs) : MyType(Rhs.MyType)
	{
		Rhs.MyType = nullptr;
	}

	template<std::derived_from<T> U>
	inline SubclassOf(const SubclassOf<U>& Rhs) : MyType(Rhs.MyType)
	{
	}

	template<std::derived_from<T> U>
	inline SubclassOf(SubclassOf<U>&& Rhs) : MyType(Rhs.MyType)
	{
		Rhs.MyType = nullptr;
	}

	inline SubclassOf(Type* NewType) : MyType(NewType)
	{
		check(NewType == nullptr || NewType->IsDerivedFrom<T>());
	}

	inline bool IsValid() const
	{
		return MyType != nullptr;
	}

	inline Type* Get()
	{
		return MyType;
	}

	inline SubclassOf& operator =(const SubclassOf& Rhs)
	{
		MyType = Rhs.MyType;
		return *this;
	}

	inline SubclassOf& operator =(SubclassOf&& Rhs)
	{
		MyType = Rhs.MyType;
		Rhs.MyType = nullptr;
		return *this;
	}

	template<std::derived_from<T> U>
	inline SubclassOf& operator =(const SubclassOf<U>& Rhs)
	{
		MyType = Rhs.MyType;
		return *this;
	}

	template<std::derived_from<T> U>
	inline SubclassOf& operator =(SubclassOf<U>&& Rhs)
	{
		MyType = Rhs.MyType;
		Rhs.MyType = nullptr;
		return *this;
	}

	inline auto operator ->() const { return MyType; }
	inline auto operator *() const { return MyType; }
	inline auto operator <=>(const SubclassOf& Rhs) const = default;

	inline operator bool() const { return IsValid(); }
	inline operator Type* () const { return MyType; }
};