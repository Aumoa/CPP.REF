// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"
#include "Platform/PlatformLocalization.h"
#include <numbers>

namespace Ayla
{
	template<std::floating_point T>
	struct Degrees;

	/// <summary>
	/// Represents the radians value.
	/// </summary>
	template<std::floating_point T>
	struct Radians
	{
		T Value = T{};

		/// <summary>
		/// Initialize new <see cref="Radians"/> instance.
		/// </summary>
		inline constexpr Radians()
		{
		}

		/// <summary>
		/// Initialize new <see cref="Radians"/> instance.
		/// </summary>
		inline constexpr Radians(const Radians& rhs)
			: Value(rhs.Value)
		{
		}

		/// <summary>
		/// Initialize new <see cref="Radians"/> instance.
		/// </summary>
		/// <param name="Value"> The initial value. </param>
		inline constexpr Radians(const T& Value)
			: Value(Value)
		{
		}

		/// <summary>
		/// Get simple string represents this radians value.
		/// </summary>
		/// <returns> The simple string value. </returns>
		inline String ToString() const
		{
			return String::Format(TEXT("{}rad"), Value);
		}

		/// <summary>
		/// Compare nearly equals to target radians with epsilon value.
		/// </summary>
		/// <param name="Rhs"> The target radians. </param>
		/// <param name="Epsilon"> The epsilon value. If different of two components is lower than this values, is nearly equals. </param>
		/// <returns> Indicate two radians is nearly equals. </returns>
		inline constexpr bool NearlyEquals(const Radians& Rhs, const T& Epsilon) const;

		/// <summary>
		/// Get clamped angle.
		/// </summary>
		/// <returns> The clamped value. </returns>
		inline constexpr Radians GetClamped() const;

		/// <summary>
		/// Get normalized angle.
		/// </summary>
		/// <returns> The normalized value. </returns>
		inline constexpr Radians GetNormal() const
		{
			Radians Clamped = GetClamped();
			if (Clamped.Value > AngleHalf)
			{
				return Clamped.Value - AngleMax;
			}
			else
			{
				return Clamped;
			}
		}

		/// <summary>
		/// Convert radians to degrees.
		/// </summary>
		/// <returns> The degrees value. </returns>
		inline constexpr Degrees<T> ToDegrees() const;

		inline constexpr bool operator ==(const Radians& Rhs) const
		{
			return Value == Rhs.Value;
		}

		inline constexpr bool operator !=(const Radians& Rhs) const
		{
			return Value != Rhs.Value;
		}

		inline constexpr auto operator <(const Radians& Rhs) const
		{
			return Value < Rhs.Value;
		}

		inline constexpr auto operator <=(const Radians& Rhs) const
		{
			return Value <= Rhs.Value;
		}

		inline constexpr auto operator >(const Radians& Rhs) const
		{
			return Value > Rhs.Value;
		}

		inline constexpr auto operator >=(const Radians& Rhs) const
		{
			return Value >= Rhs.Value;
		}

		inline Radians& operator =(const Radians& Rhs)
		{
			Value = Rhs.Value;
			return *this;
		}

		inline Radians& operator +=(const Radians& Rhs)
		{
			Value += Rhs.Value;
			return *this;
		}

		inline Radians& operator -=(const Radians& Rhs)
		{
			Value -= Rhs.Value;
			return *this;
		}

		inline Radians& operator *=(const Radians& Rhs)
		{
			Value *= Rhs.Value;
			return *this;
		}

		inline Radians& operator /=(const Radians& Rhs)
		{
			Value /= Rhs.Value;
			return *this;
		}

		inline constexpr Radians operator -() const
		{
			return Radians(-Value);
		}

		inline constexpr Radians operator +(const Radians& Rhs) const
		{
			return Radians(Value + Rhs.Value);
		}

		inline constexpr Radians operator -(const Radians& Rhs) const
		{
			return Radians(Value - Rhs.Value);
		}

		inline constexpr Radians operator *(const Radians& Rhs) const
		{
			return Radians(Value * Rhs.Value);
		}

		inline constexpr Radians operator /(const Radians& Rhs) const
		{
			return Radians(Value / Rhs.Value);
		}

	private:
		static constexpr T AngleHalf = std::numbers::pi_v<T>;
		static constexpr T AngleMax = AngleHalf * 2.0f;
		static constexpr T _180PI = 180.0f / std::numbers::pi_v<T>;
	};

	using RadiansF = Radians<float>;
	using RadiansD = Radians<double>;
}