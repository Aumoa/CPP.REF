
// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Object.h"

template<class T>
concept THasLerp = requires()
{
	{ T::Lerp(T(), T(), (float)0) };
};

template<class T>
constexpr bool VHasLerp = false;
template<THasLerp T>
constexpr bool VHasLerp<T> = true;

template<TIsRealType T>
struct TRadians;
template<TIsRealType T>
struct TDegrees;
struct Vector4;
struct Vector3;
struct Vector2;

template<class T>
concept TOverrideMinMax = requires()
{
	{ T::Min(T(), T()) };
	{ T::Max(T(), T()) };
};

class Math abstract final : virtual public Object
{
public:
	using Super = Object;
	using This = Math;

public:
	CORE_API static TRadians<float> Sin(float value);
	CORE_API static TRadians<float> Cos(float value);
	CORE_API static TRadians<float> Tan(float value);
	CORE_API static TRadians<float> Asin(float value);
	CORE_API static TRadians<float> Acos(float value);
	CORE_API static TRadians<float> Atan(float value);
	CORE_API static TRadians<float> Atan2(float y, float x);
	CORE_API static std::pair<float, float> SinCos(TRadians<float> value);
	CORE_API static float InvSqrt(float x);

	template<class T>
	inline static constexpr T Lerp(const T& left, const T& right, float t);

	template<class T>
	inline static constexpr T Square(const T& value);

	template<class T>
	inline static constexpr T Mod(const T& value, const T& divisor);

	template<class T>
	inline static constexpr T Abs(const T& value);

	template<class T, class... TRest>
	inline static constexpr T Min(const T& lh, const T& rh, TRest&&... rhs);

	template<TOverrideMinMax T, class... TRest>
	inline static constexpr T Min(const T& lh, const T& rh, TRest&&... rhs);

	template<class T, class... TRest>
	inline static constexpr T Max(const T& lh, const T& rh, TRest&&... rhs);

	template<TOverrideMinMax T, class... TRest>
	inline static constexpr T Max(const T& lh, const T& rh, TRest&&... rhs);

	template<class T>
	inline static constexpr T Clamp(const T& value, const T& start, const T& end);

	template<class T>
	inline static constexpr bool InRange(const T& value, const T& start, const T& end);

	template<class T = float>
	inline static constexpr const T PI = (T)3.141592;
	template<class T = float>
	inline static constexpr const T HalfPI = PI<T> / (T)2;
	template<class T = float>
	inline static constexpr const T InvPI = (T)1 / PI<T>;
	template<class T = float>
	inline static constexpr const T SmallNumber = (T)1.e-6;
};

#include "Math.inl"