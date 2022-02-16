// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "CoreConcepts.h"
#include "Misc/RecursiveMacroHelper.h"

namespace Enum
{
	template<size_t N>
	struct Cstr
	{
		char Str[N];
		size_t Size = N;

		constexpr Cstr(const char(&Str)[N])
		{
			std::copy_n(Str, N, this->Str);
		}
	};

	template<std::integral T, Cstr ConstString>
	struct KeyValuePair
	{
		T Value;

		constexpr KeyValuePair(T&& V) : Value(V)
		{
		}

		static constexpr std::string_view ToString()
		{
			return std::string_view(ConstString.Str);
		}
	};

	template<std::integral T>
	struct EEnumBase
	{
		using Type = T;

		T Value;

		EEnumBase() : Value(0)
		{
		}

		constexpr EEnumBase(const T& Value) : Value(Value)
		{
		}

		constexpr explicit operator const T&() const
		{
			return Value;
		}

		constexpr EEnumBase& operator =(const T& Value)
		{
			this->Value = Value;
			return *this;
		}

		template<Cstr ConstString>
		using KeyValuePair = KeyValuePair<T, ConstString>;
	};
}

#define SENUM_DECLARE_ENUM_ELEMENT(Name, Value) Name = Value,
#define SENUM_DECLARE_CONSTEXPR_ELEMENT(Name, Value) static constexpr KeyValuePair<#Name> Name = (Type)EEnum::Name;

#define SENUM(Name, Type, ...)																		\
struct Name : public Enum::EEnumBase<Type>															\
{																									\
	enum class EEnum : Type																			\
	{																								\
		MACRO_RECURSIVE_FOR_EACH_2(SENUM_DECLARE_ENUM_ELEMENT, __VA_ARGS__)							\
	};																								\
																									\
	MACRO_RECURSIVE_FOR_EACH_2(SENUM_DECLARE_CONSTEXPR_ELEMENT, __VA_ARGS__);						\
};
