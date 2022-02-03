// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "CoreConcepts.h"
#include <any>
#include <sstream>
#include "Misc/StringUtils.h"

class SValueType : implements SObject
{
private:
	std::any _value;
	std::wstring _toString;
	Type* _type = nullptr;

public:
	template<class T>
	SValueType(const T& value) : SObject()
		, _value(value)
	{
		_toString = Internal_ToString(value, InheritSelector<6>());
		_type = Type::GetStaticClass<T>();
	}

	virtual ~SValueType() noexcept override
	{
	}

	virtual Type* GetType() const override
	{
		return _type;
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

public:
	template<class T>
	inline static std::wstring Internal_ToString(const T& value, InheritSelector<0>&&)
	{
		return ANSI_TO_WCHAR(typeid(T).name());
	}

	template<std::convertible_to<std::string> T>
	inline static std::wstring Internal_ToString(const T& value, InheritSelector<1>&&)
	{
		return ANSI_TO_WCHAR((std::string)value);
	}

	template<class T>
	inline static std::wstring Internal_ToString(const T& value, InheritSelector<2>&&) requires requires { std::to_string(value); }
	{
		return ANSI_TO_WCHAR(std::to_string(value));
	}

	template<class T>
	inline static std::wstring Internal_ToString(const T& value, InheritSelector<3>&&) requires requires { std::declval<std::ostringstream>() << value; }
	{
		std::ostringstream os;
		os << value;
		return ANSI_TO_WCHAR(os.str());
	}

	template<std::convertible_to<std::wstring> T>
	inline static std::wstring Internal_ToString(const T& value, InheritSelector<4>&&)
	{
		return (std::wstring)value;
	}

	template<class T>
	inline static std::wstring Internal_ToString(const T& value, InheritSelector<5>&&) requires requires { std::to_wstring(value); }
	{
		return std::to_wstring(value);
	}

	template<class T>
	inline static std::wstring Internal_ToString(const T& value, InheritSelector<6>&&) requires requires { std::declval<std::wostringstream>() << value; }
	{
		std::wostringstream os;
		os << value;
		return os.str();
	}
};