// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <string>
#include <format>
#include <numbers>
#include "Misc/StringUtils.h"
#include "MathEx.h"

struct Degrees;

/// <summary>
/// Represents the radians value.
/// </summary>
struct Radians
{
	float Value = 0;

	/// <summary>
	/// Initialize new <see cref="Radians"/> instance.
	/// </summary>
	inline constexpr Radians()
	{
	}

	/// <summary>
	/// Initialize new <see cref="Radians"/> instance.
	/// </summary>
	/// <param name="value"> The initial value. </param>
	inline constexpr Radians(float value) : Value(value)
	{
	}

	/// <summary>
	/// Get simple string represents this radians value.
	/// </summary>
	/// <param name="formatArgs"> The formatting args that use to std::format. </param>
	/// <returns> The simple string value. </returns>
	inline std::wstring ToString(std::wstring_view formatArgs = L"") const
	{
		std::wstring placeholder = StringUtils::GetPlaceholder(formatArgs) + L"rad";
		return std::format(placeholder, Value);
	}

	/// <summary>
	/// Compare nearly equals to target radians with epsilon value.
	/// </summary>
	/// <param name="rhs"> The target radians. </param>
	/// <param name="epsilon"> The epsilon value. If different of two components is lower than this values, is nearly equals. </param>
	/// <returns> Indicate two radians is nearly equals. </returns>
	inline constexpr bool NearlyEquals(const Radians& rhs, float epsilon) const
	{
		return MathEx::Abs(Value - rhs.Value) <= epsilon;
	}

	/// <summary>
	/// Get clamped angle.
	/// </summary>
	/// <returns> The clamped value. </returns>
	inline constexpr Radians GetClamped() const
	{
		return MathEx::Mod(Value, AngleMax);
	}

	/// <summary>
	/// Get normalized angle.
	/// </summary>
	/// <returns> The normalized value. </returns>
	inline constexpr Radians GetNormal() const
	{
		Radians clamped = GetClamped();
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
	/// Convert radians to degrees.
	/// </summary>
	/// <returns> The degrees value. </returns>
	inline constexpr Degrees ToDegrees() const;

	inline constexpr bool operator ==(const Radians& rhs) const
	{
		return Value == rhs.Value;
	}

	inline constexpr bool operator !=(const Radians& rhs) const
	{
		return Value != rhs.Value;
	}

	inline constexpr auto operator <(const Radians& rhs) const
	{
		return Value < rhs.Value;
	}

	inline constexpr auto operator <=(const Radians& rhs) const
	{
		return Value <= rhs.Value;
	}

	inline constexpr auto operator >(const Radians& rhs) const
	{
		return Value > rhs.Value;
	}

	inline constexpr auto operator >=(const Radians& rhs) const
	{
		return Value >= rhs.Value;
	}

	inline Radians& operator =(const Radians& rhs)
	{
		Value = rhs.Value;
		return *this;
	}

	inline Radians& operator +=(const Radians& rhs)
	{
		Value += rhs.Value;
		return *this;
	}

	inline Radians& operator -=(const Radians& rhs)
	{
		Value -= rhs.Value;
		return *this;
	}

	inline Radians& operator *=(const Radians& rhs)
	{
		Value *= rhs.Value;
		return *this;
	}

	inline Radians& operator /=(const Radians& rhs)
	{
		Value /= rhs.Value;
		return *this;
	}

	inline constexpr Radians operator -() const
	{
		return Radians(-Value);
	}

	inline constexpr Radians operator +(const Radians& rhs) const
	{
		return Radians(Value + rhs.Value);
	}

	inline constexpr Radians operator -(const Radians& rhs) const
	{
		return Radians(Value - rhs.Value);
	}

	inline constexpr Radians operator *(const Radians& rhs) const
	{
		return Radians(Value * rhs.Value);
	}

	inline constexpr Radians operator /(const Radians& rhs) const
	{
		return Radians(Value / rhs.Value);
	}

private:
	static constexpr float AngleHalf = std::numbers::pi_v<float>;
	static constexpr float AngleMax = AngleHalf * 2.0f;
	static constexpr float _180PI = 180.0f / std::numbers::pi_v<float>;
};