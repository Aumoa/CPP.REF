// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "CoreConcepts.h"
#include "Misc/String.h"
#include "typeof.h"
#include <any>
#include <sstream>

class SValueType : virtual public SObject
{
private:
	std::any _value;
	std::wstring _toString;
	SType* _type = nullptr;

public:
	template<class T>
	SValueType(const T& value) : SObject()
		, _value(value)
	{
		_type = typeof(T);
		_toString = Internal_ToString(0, value);
	}

	virtual ~SValueType() noexcept override
	{
	}

	virtual SType* GetType() const override
	{
		return _type;
	}

	virtual std::wstring ToString() override
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

private:
	template<class T>
	inline static std::wstring Internal_ToString(int, const T& value) requires
		requires { std::declval<T>().ToString(); }
	{
		return value.ToString();
	}

	template<class T>
	inline static std::wstring Internal_ToString(short, const T& value)
	{
		return String::AsUnicode(typeid(T).name());
	}
};