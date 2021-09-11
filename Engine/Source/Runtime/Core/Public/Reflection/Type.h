// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <typeinfo>
#include <functional>
#include "PrimitiveTypes.h"

class Object;

class CORE_API Type
{
	using ObjectCtor = std::function<Object*()>;

	std::wstring_view _friendlyName;
	ObjectCtor _ctor;

public:
	template<class TType>
	struct TypeGenerator
	{
		std::wstring_view FriendlyName;

		TypeGenerator(std::wstring_view friendlyName) : FriendlyName(friendlyName)
		{
		}
	};

public:
	template<class TType>
	Type(TypeGenerator<TType>&& generator)
		: _friendlyName(generator.FriendlyName)
		, _ctor(GetConstructorFunctionBody<TType>((int32)0))
	{
	}

	std::wstring_view GetFriendlyName() const;
	Object* Instantiate() const;

private:
	template<class TType> requires std::constructible_from<TType>
	static ObjectCtor GetConstructorFunctionBody(int32)
	{
		return []() -> Object*
		{
			return new TType();
		};
	}

	template<class TType>
	static ObjectCtor GetConstructorFunctionBody(float)
	{
		return nullptr;
	}
};