// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "CoreConcepts.h"
#include "Misc/RecursiveMacroHelper.h"
#include <map>

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

	template<class TTag, std::integral T, Cstr ConstString>
	struct KeyValuePair
	{
		using __Tag__ = TTag;

		T Value;

		constexpr KeyValuePair(T&& V) : Value(V)
		{
		}

		static constexpr std::string_view ToString()
		{
			return std::string_view(ConstString.Str);
		}

		template<std::integral U>
		explicit constexpr operator U() const
		{
			return static_cast<U>(Value);
		}

		template<class U>
		explicit constexpr operator U() const requires std::is_enum_v<U>
		{
			return static_cast<U>(Value);
		}

		constexpr operator __Tag__() const
		{
			return static_cast<__Tag__>(Value);
		}
	};

	template<class TBase, std::integral T>
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

		template<std::integral U>
		explicit constexpr operator U() const
		{
			return static_cast<U>(Value);
		}

		template<class U>
		explicit constexpr operator U() const requires std::is_enum_v<U>
		{
			return static_cast<U>(Value);
		}

		constexpr EEnumBase& operator =(const T& Value)
		{
			this->Value = Value;
			return *this;
		}

		template<class TTag, Cstr ConstString>
		using MyKeyValuePair = KeyValuePair<TTag, T, ConstString>;

		constexpr auto operator <=>(const EEnumBase& Rhs) const = default;
	};
}

#define SENUM_DECLARE_ENUM_ELEMENT(Name, ...) Name __VA_OPT__(= __VA_ARGS__),
#define SENUM_DECLARE_CONSTEXPR_ELEMENT(Name, ...) static constexpr MyKeyValuePair<__Tag__, #Name> Name = (Type)EEnum::Name;
#define SENUM_DECLARE_DICTIONARY_PAIR_PARSE(Name, ...) { L ## #Name, __Type__(Name) },
#define SENUM_DECLARE_DICTIONARY_PAIR_TOSTRING(Name, ...) { Name.Value, L ## #Name },

#define SENUM(Name, Type, ...)																		\
struct Name : public Enum::EEnumBase<Name, Type>													\
{																									\
	using __Type__ = Name;																			\
																									\
	enum class EEnum : Type																			\
	{																								\
		MACRO_RECURSIVE_FOR_EACH_2(SENUM_DECLARE_ENUM_ELEMENT, __VA_ARGS__)							\
	};																								\
																									\
	using __Tag__ = EEnum;																			\
																									\
	MACRO_RECURSIVE_FOR_EACH_2(SENUM_DECLARE_CONSTEXPR_ELEMENT, __VA_ARGS__);						\
																									\
	Name()																							\
	{																								\
	}																								\
																									\
	constexpr explicit Name(const Type& Value) : EEnumBase<Name, Type>(Value)						\
	{																								\
	}																								\
																									\
	template<class TKeyValuePair>																	\
	constexpr Name(const TKeyValuePair& V) requires													\
		std::same_as<typename TKeyValuePair::__Tag__, __Tag__>										\
		: EEnumBase<Name, Type>(V.Value)															\
	{																								\
	}																								\
																									\
	static bool TryParse(std::wstring_view EnumName, Name& OutValue)								\
	{																								\
		static std::map<std::wstring_view, Name, std::less<>> Dictionary =							\
		{																							\
			MACRO_RECURSIVE_FOR_EACH_2(SENUM_DECLARE_DICTIONARY_PAIR_PARSE, __VA_ARGS__)			\
		};																							\
																									\
		if (auto It = Dictionary.find(EnumName); It != Dictionary.end())							\
		{																							\
			OutValue = It->second;																	\
			return true;																			\
		}																							\
		else																						\
		{																							\
			return false;																			\
		}																							\
	}																								\
																									\
	std::wstring ToString() const																	\
	{																								\
		static std::map<Type, std::wstring> Dictionary =											\
		{																							\
			MACRO_RECURSIVE_FOR_EACH_2(SENUM_DECLARE_DICTIONARY_PAIR_TOSTRING, __VA_ARGS__)			\
		};																							\
																									\
		if (auto It = Dictionary.find(this->Value); It != Dictionary.end())							\
		{																							\
			return It->second;																		\
		}																							\
		else																						\
		{																							\
			return std::format(L"(" L ## #Name L"){}", this->Value);								\
		}																							\
	}																								\
																									\
	template<class TKeyValuePair>																	\
	constexpr auto operator ==(const TKeyValuePair& Rhs) const										\
	{ return operator <=>(Rhs.Value) == 0; }														\
	template<class TKeyValuePair>																	\
	constexpr auto operator !=(const TKeyValuePair& Rhs) const										\
	{ return operator <=>(Rhs.Value) != 0; }														\
	template<class TKeyValuePair>																	\
	constexpr auto operator <=(const TKeyValuePair& Rhs) const										\
	{ return operator <=>(Rhs.Value) <= 0; }														\
	template<class TKeyValuePair>																	\
	constexpr auto operator >=(const TKeyValuePair& Rhs) const										\
	{ return operator <=>(Rhs.Value) >= 0; }														\
	template<class TKeyValuePair>																	\
	constexpr auto operator < (const TKeyValuePair& Rhs) const										\
	{ return operator <=>(Rhs.Value) <  0; }														\
	template<class TKeyValuePair>																	\
	constexpr auto operator > (const TKeyValuePair& Rhs) const										\
	{ return operator <=>(Rhs.Value) >  0; }														\
																									\
	constexpr operator __Tag__() const																\
	{																								\
		return static_cast<__Tag__>(this->Value);													\
	}																								\
};
