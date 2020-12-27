// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Object.h"

#include <cmath>

namespace SC::Runtime::Core::Mathematics
{
	template<class T>
	concept THasLerp = requires()
	{
		{ T::Lerp(T(), T(), (double)0) };
	};

	template<class T>
	constexpr bool VHasLerp = false;
	template<THasLerp T>
	constexpr bool VHasLerp<T> = true;

	template<TIsRealType T>
	struct TRadians;
	template<TIsRealType T>
	struct TDegrees;

	class CORE_API Math abstract final : virtual public Object
	{
	public:
		using Super = Object;
		using This = Math;

		template<class T>
		inline static T Lerp(const T& left, const T& right, double t);
		template<class T>
		inline static T Square(const T& value);
		template<class TItem>
		inline static TRadians<TItem> Atan2(TItem y, TItem x);
		template<class TItem>
		inline static TRadians<TItem> Asin(TItem value);
		template<TIsRealType T>
		inline static std::pair<T, T> SinCos(TRadians<T> value);

		template<class T>
		inline static constexpr T Mod(const T& value, const T& divisor);
		template<class T>
		inline static constexpr T Abs(const T& value);

		template<class T = double>
		inline static constexpr const T PI = (T)3.141592;
		template<class T = double>
		inline static constexpr const T HalfPI = PI<T> / (T)2;
		template<class T = double>
		inline static constexpr const T InvPI = (T)1 / PI<T>;
	};
}