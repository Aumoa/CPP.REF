// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"
#include "Radians.h"
#include "Platform/PlatformLocalization.h"
#include <numbers>

namespace Ayla
{
	/// <summary>
	/// Represents the degrees value.
	/// </summary>
	template<std::floating_point T>
	struct Degrees
	{
		T Value = T{};

		/// <summary>
		/// Initialize new <see cref="Degrees"/> instance.
		/// </summary>
		inline constexpr Degrees()
		{
		}

		/// <summary>
		/// Initialize new <see cref="Degrees"/> instance.
		/// </summary>
		inline constexpr Degrees(const Degrees& rhs)
			: Value(rhs.Value)
		{
		}

		/// <summary>
		/// Initialize new <see cref="Degrees"/> instance.
		/// </summary>
		/// <param name="Value"> The initial value. </param>
		inline constexpr Degrees(const T& Value)
			: Value(Value)
		{
		}

		/// <summary>
		/// Get simple string represents this degrees value.
		/// </summary>
		/// <returns> The simple string value. </returns>
		inline String ToString() const
		{
			return String::Format(TEXT("{}°"), Value);
		}

		/// <summary>
		/// Compare nearly equals to target degrees with epsilon value.
		/// </summary>
		/// <param name="Rhs"> The target degrees. </param>
		/// <param name="Epsilon"> The epsilon value. If different of two components is lower than this values, is nearly equals. </param>
		/// <returns> Indicate two degrees is nearly equals. </returns>
		inline constexpr bool NearlyEquals(const Degrees& Rhs, const T& Epsilon) const;

		/// <summary>
		/// Get clamped angle.
		/// </summary>
		/// <returns> The clamped value. </returns>
		inline constexpr Degrees GetClamped() const;

		/// <summary>
		/// Get normalized angle.
		/// </summary>
		/// <returns> The normalized value. </returns>
		inline constexpr Degrees GetNormal() const
		{
			Degrees clamped = GetClamped();
			if (clamped.Value > AngleHalf)
			{
				return clamped.Value - AngleMax;
			}
			else
			{
				return clamped;
			}
		}

		/// <summary>
		/// Convert degrees to radians.
		/// </summary>
		/// <returns> The degrees value. </returns>
		inline constexpr Radians<T> ToRadians() const;

		inline constexpr bool operator ==(const Degrees& Rhs) const
		{
			return Value == Rhs.Value;
		}

		inline constexpr bool operator !=(const Degrees& Rhs) const
		{
			return Value != Rhs.Value;
		}

		inline constexpr auto operator <(const Degrees& Rhs) const
		{
			return Value < Rhs.Value;
		}

		inline constexpr auto operator <=(const Degrees& Rhs) const
		{
			return Value <= Rhs.Value;
		}

		inline constexpr auto operator >(const Degrees& Rhs) const
		{
			return Value > Rhs.Value;
		}

		inline constexpr auto operator >=(const Degrees& Rhs) const
		{
			return Value >= Rhs.Value;
		}

		inline Degrees& operator =(const Degrees& Rhs)
		{
			Value = Rhs.Value;
			return *this;
		}

		inline Degrees& operator +=(const Degrees& Rhs)
		{
			Value += Rhs.Value;
			return *this;
		}

		inline Degrees& operator -=(const Degrees& Rhs)
		{
			Value -= Rhs.Value;
			return *this;
		}

		inline Degrees& operator *=(const Degrees& Rhs)
		{
			Value *= Rhs.Value;
			return *this;
		}

		inline Degrees& operator /=(const Degrees& Rhs)
		{
			Value /= Rhs.Value;
			return *this;
		}

		inline constexpr Degrees operator -() const
		{
			return Degrees(-Value);
		}

		inline constexpr Degrees operator +(const Degrees& Rhs) const
		{
			return Degrees(Value + Rhs.Value);
		}

		inline constexpr Degrees operator -(const Degrees& Rhs) const
		{
			return Degrees(Value - Rhs.Value);
		}

		inline constexpr Degrees operator *(const Degrees& Rhs) const
		{
			return Degrees(Value * Rhs.Value);
		}

		inline constexpr Degrees operator /(const Degrees& Rhs) const
		{
			return Degrees(Value / Rhs.Value);
		}

	private:
		static constexpr T AngleHalf = 180.0f;
		static constexpr T AngleMax = AngleHalf * 2.0f;
		static constexpr T PI180 = std::numbers::pi_v<T> / 180.0f;
	};

	using DegreesF = Degrees<float>;
	using DegreesD = Degrees<double>;
}