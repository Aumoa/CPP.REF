// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

namespace SC::Runtime::Core::Mathematics
{
	template<TIsRealType T>
	struct TRadians
	{
	private:
		static constexpr T PI = (T)3.141592;
		static constexpr T Inv180 = (T)180.0 / PI;

	public:
		T Value;

		inline TRadians()
		{

		}

		inline constexpr TRadians(const T& value) : Value(value)
		{

		}

		inline constexpr TRadians(const TRadians& value) : Value(value.Value)
		{

		}

		inline constexpr TDegrees<T> ToDegrees() const;

		inline TRadians& operator =(const TRadians& value)
		{
			Value = value.Value;
			return *this;
		}

		inline TRadians& operator +=(const TRadians& value)
		{
			Value += value.Value;
			return *this;
		}

		inline TRadians& operator -=(const TRadians& value)
		{
			Value -= value.Value;
			return *this;
		}

		inline TRadians& operator *=(const T& value)
		{
			Value *= value;
			return *this;
		}

		inline TRadians& operator /=(const T& value)
		{
			Value /= value;
			return *this;
		}

		inline constexpr TRadians operator +(const TRadians& value) const
		{
			return TRadians(Value + value.Value);
		}

		inline constexpr TRadians operator -(const TRadians& value) const
		{
			return TRadians(Value - value.Value);
		}

		inline constexpr TRadians operator *(const T& value) const
		{
			return TRadians(Value * value);
		}

		inline constexpr TRadians operator /(const T& value) const
		{
			return TRadians(Value / value);
		}

		template<TIsRealType O>
		inline constexpr operator TRadians<O>() const
		{
			return TRadians<O>(static_cast<O>(Value));
		}

		inline constexpr bool operator ==(const TRadians& rh) const
		{
			return Value == rh.Value;
		}

		inline constexpr bool operator !=(const TRadians& rh) const
		{
			return Value == rh.Value;
		}

		inline constexpr bool operator <(const TRadians& rh) const
		{
			return Value < rh.Value;
		}

		inline constexpr bool operator >(const TRadians& rh) const
		{
			return Value > rh.Value;
		}

		inline constexpr bool operator <=(const TRadians& rh) const
		{
			return Value <= rh.Value;
		}

		inline constexpr bool operator >=(const TRadians& rh) const
		{
			return Value >= rh.Value;
		}
	};

	template<TIsRealType T>
	inline constexpr TRadians<T> operator *(const T& lh, const TRadians<T>& rh)
	{
		return TRadians<T>(lh * rh.Value);
	}
}