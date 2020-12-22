// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

namespace SC::Runtime::Core::Mathematics
{
	template<TIsRealType T>
	struct TRadians;

	template<TIsRealType T>
	struct TDegrees
	{
	private:
		static constexpr T PI = (T)3.141592;
		static constexpr T InvPI = PI / (T)180.0;

	public:
		T Value;

		inline TDegrees()
		{

		}

		inline constexpr TDegrees(const T& value) : Value(value)
		{

		}

		inline constexpr TDegrees(const TDegrees& value) : Value(value.Value)
		{

		}

		inline constexpr TRadians<T> ToRadians() const;

		inline TDegrees& operator =(const TDegrees& value)
		{
			Value = value.Value;
			return *this;
		}

		inline TDegrees& operator +=(const TDegrees& value)
		{
			Value += value.Value;
			return *this;
		}

		inline TDegrees& operator -=(const TDegrees& value)
		{
			Value -= value.Value;
			return *this;
		}

		inline TDegrees& operator *=(const T& value)
		{
			Value *= value;
			return *this;
		}

		inline TDegrees& operator /=(const T& value)
		{
			Value /= value;
			return *this;
		}

		inline constexpr TDegrees operator +(const TDegrees& value) const
		{
			return TDegrees(Value + value.Value);
		}

		inline constexpr TDegrees operator -(const TDegrees& value) const
		{
			return TDegrees(Value - value.Value);
		}

		inline constexpr TDegrees operator *(const T& value) const
		{
			return TDegrees(Value * value);
		}

		inline constexpr TDegrees operator /(const T& value) const
		{
			return TDegrees(Value / value);
		}

		template<TIsRealType O>
		inline constexpr operator TDegrees<O>() const
		{
			return TDegrees<O>(static_cast<O>(Value));
		}

		inline constexpr bool operator ==(const TDegrees& rh) const
		{
			return Value == rh.Value;
		}

		inline constexpr bool operator !=(const TDegrees& rh) const
		{
			return Value == rh.Value;
		}

		inline constexpr bool operator <(const TDegrees& rh) const
		{
			return Value < rh.Value;
		}

		inline constexpr bool operator >(const TDegrees& rh) const
		{
			return Value > rh.Value;
		}

		inline constexpr bool operator <=(const TDegrees& rh) const
		{
			return Value <= rh.Value;
		}

		inline constexpr bool operator >=(const TDegrees& rh) const
		{
			return Value >= rh.Value;
		}
	};

	template<TIsRealType T>
	inline constexpr TDegrees<T> operator *(const T& lh, const TDegrees<T>& rh)
	{
		return TDegrees<T>(lh * rh.Value);
	}
}