// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "LogCore.h"
#include "ObjectBase.h"
#include "CoreConcepts.h"
#include "Reflection/ReflectionMacros.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogVerbosity.h"
#include <optional>

class SObject;
class SValueType;

template<std::derived_from<SObject> TTo, std::derived_from<SObject> TFrom>
inline TTo* Cast(TFrom* InFrom)
{
	return dynamic_cast<TTo*>(InFrom);
}

template<std::same_as<SObject> TTo, class TFrom>
inline TTo* Cast(const TFrom& InValue) requires (!std::derived_from<TFrom, SObject>)
{
	return gcnew SValueType(InValue);
}

template<class TTo, std::same_as<SObject> TFrom>
inline TTo Cast(TFrom* InValue) requires (!std::derived_from<TTo, SObject>) && (!requires(const TTo& IsOpt)
{
	{ IsOpt.has_value() } -> std::same_as<bool>;
	{ *IsOpt };
})
{
	auto ValueTypePtr = Cast<SValueType>(InValue);
	if (ValueTypePtr == nullptr)
	{
		SE_LOG(LogCasts, Fatal, L"Object is not boxing class.");
	}

	TTo OutValue;
	if (!ValueTypePtr->Unboxing(&OutValue))
	{
		SE_LOG(LogCasts, Fatal, L"The type of value contained at boxing object is not match with desired type.");
	}

	return OutValue;
}

template<class TTo, std::same_as<SObject> TFrom>
inline TTo Cast(TFrom* InValue) requires (!std::derived_from<TTo, SObject>) && requires(const TTo& IsOpt)
{
	{ IsOpt.has_value() } -> std::same_as<bool>;
	{ *IsOpt };
}
{
	using ValueT = std::remove_reference_t<decltype(*std::declval<TTo>())>;

	auto ValueTypePtr = Cast<SValueType>(InValue);
	if (ValueTypePtr == nullptr)
	{
		return std::nullopt;
	}

	ValueT OutValue;
	if (!ValueTypePtr->Unboxing(&OutValue))
	{
		return std::nullopt;
	}

	return OutValue;
}