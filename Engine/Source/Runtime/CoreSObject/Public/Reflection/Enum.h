// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "CoreConcepts.h"
#include "Object.h"
#include "ReflectionMacros.h"
#include "ReflectionTypeTraits.h"
#include "Misc/RecursiveMacroHelper.h"
#include "Misc/String.h"
#include <map>

namespace libty::Enum
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

	template<class TBase, std::integral T> requires
		(sizeof(T) <= sizeof(int64))
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
#define SENUM_DECLARE_CONSTEXPR_ELEMENT(Name, ...) static constexpr MyKeyValuePair<__Tag__, #Name> Name = (Type)__Tag__::Name;
#define SENUM_DECLARE_DICTIONARY_PAIR_PARSE(Name, ...) { L ## #Name, __Type__(Name) },
#define SENUM_DECLARE_DICTIONARY_PAIR_TOSTRING(Name, ...) { Name.Value, L ## #Name },

#define SENUM_BEGIN(Name, Type, ...)																\
struct Name : public libty::Enum::EEnumBase<Name, Type>												\
{																									\
	using __Type__ = Name;																			\
																									\
	enum class __Tag__ : Type																		\
	{																								\
		MACRO_RECURSIVE_FOR_EACH_2(SENUM_DECLARE_ENUM_ELEMENT, __VA_ARGS__)							\
	};																								\
																									\
	MACRO_RECURSIVE_FOR_EACH_2(SENUM_DECLARE_CONSTEXPR_ELEMENT, __VA_ARGS__);						\
																									\
	GENERATED_BODY(Name);																			\
																									\
public:																								\
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
			return String::Format(L"(" L ## #Name L"){}", this->Value);								\
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
	}

#define SENUM_END() \
};

#define SENUM(Name, Type, ...) \
SENUM_BEGIN(Name, Type __VA_OPT__(, __VA_ARGS__)) \
SENUM_END();

namespace std
{
	template<libty::Core::Reflection::IEnum TEnum>
	struct underlying_type<TEnum>
	{
		using type = typename std::underlying_type<typename TEnum::__Tag__>::type;
	};

	template<libty::Core::Reflection::IEnum TEnum>
	struct is_enum<TEnum> : public bool_constant<true>
	{
	};
}

class CORESOBJECT_API SEnum : virtual public SObject
{
	GENERATED_BODY(SEnum);
	SEnum() = delete;

public:
	/// <summary>
	/// Converts the string representation of the name or numeric value of one or more enumerated constants to an equivalent enumerated object. The return value indicates whether the conversion succeeded.
	/// </summary>
	/// <param name="type"> The enum type to use for parsing. </param>
	/// <param name="format"> The string representation of the name or numeric value of enumerated constants. </param>
	/// <param name="result"> When this method returns true, contains an enumeration constant that represents the parsed value. </param>
	/// <returns> true if the conversion succeeded; false otherwise. </returns>
	static bool TryParse(SType* type, std::wstring_view format, SObject*& result);	

	/// <summary>
	/// Converts the string representation of the name or numeric value of one or more enumerated constants to an equivalent enumerated object. The return value indicates whether the conversion succeeded.
	/// </summary>
	/// <typeparam name="TEnum"> The enumeration type to which to convert value. </typeparam>
	/// <param name="format"> The string representation of the name or numeric value of enumerated constants. </param>
	/// <param name="result"> When this method returns, contains an object of type TEnum whose value is represented by value if the parse operation succeeds. If the parse operation fails, contains the default value of the underlying type of TEnum. This value need not be a member of the TEnum enumeration. This parameter is passed uninitialized. </param>
	/// <returns> true if the conversion succeeded; false otherwise. </returns>
	template<libty::Core::Reflection::IEnum TEnum>
	static bool TryParse(std::wstring_view format, TEnum& result)
	{
		int64 uvalue;
		if (TryParse(TEnum::TypeId, format, uvalue))
		{
			result = TEnum((typename std::underlying_type<TEnum>::type)uvalue);
			return true;
		}
		return false;
	}

private:
	static bool TryParse(SType* type, std::wstring_view format, int64& result);
};