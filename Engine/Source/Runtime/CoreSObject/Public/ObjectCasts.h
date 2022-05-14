// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "gcnew.h"
#include <optional>

namespace libty::inline Core
{
	class SObject;
	class SValueType;

	inline namespace Casts
	{
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
				throw Exception(TEXT("Object is not boxing class."));
			}

			TTo OutValue;
			if (!ValueTypePtr->Unboxing(&OutValue))
			{
				throw Exception(TEXT("The type of value contained at boxing object is not match with desired type."));
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
	}
}