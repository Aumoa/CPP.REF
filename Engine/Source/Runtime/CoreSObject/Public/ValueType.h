// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "typeof.h"
#include <any>
#include <sstream>

namespace libty::inline Core
{
	class SValueType : implements(SObject)
	{
	private:
		std::any _value;
		String _toString;
		SType* _type = nullptr;
		bool _isGeneric = false;

	public:
		template<class T>
		SValueType(const T& value)
			: SObject()
			, _value(value)
		{
			_type = typeof(T);
			_toString = Internal_ToString(0, value);
		}

		template<template<class...> class TGenericClass, class... TArguments>
		SValueType(const TGenericClass<TArguments...>& value) requires
			IGenericClass<TGenericClass, TArguments...>
			: SObject()
			, _value(TGenericClass<void>(value))
		{
			_type = typeof(TGenericClass<>);
			_toString = Internal_ToString(0, value);
		}

		virtual ~SValueType() noexcept override
		{
		}

		virtual SType* GetType() const override
		{
			return _type;
		}

		virtual String ToString() override
		{
			return _toString;
		}

		template<class T>
		bool Unboxing(T* outValue) const
		{
			if (const T* p = std::any_cast<T>(&_value); p)
			{
				*outValue = *p;
				return true;
			}
			return false;
		}

		template<template<class...> class TGenericClass, class... TArguments>
		bool Unboxing(TGenericClass<TArguments...>* outValue) const requires
			IGenericClass<TGenericClass, TArguments...>
		{
			using T = TGenericClass<TArguments...>;

			if (_isGeneric)
			{
				if (const T* p = std::any_cast<T>(&_value); p)
				{
					*outValue = *p;
					return true;
				}
				return false;
			}
			else
			{
				TGenericClass<void> transp;
				if (const TGenericClass<void>* p = std::any_cast<TGenericClass<void>>(&_value); p)
				{
					*outValue = TGenericClass<TArguments...>(*p);
					return true;
				}
				return false;
			}
		}

	private:
		template<class T>
		inline static String Internal_ToString(int, const T& value) requires
			requires { std::declval<T>().ToString(); }
		{
			return value.ToString();
		}

		template<class T>
		inline static String Internal_ToString(short, const T& value)
		{
			return String::FromLiteral(typeid(T).name());
		}
	};
}