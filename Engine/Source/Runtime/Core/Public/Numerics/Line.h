// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Vector.h"

/// <summary>
/// Represents line that contains start point and end point.
/// </summary>
template<size_t N>
struct Line
{
	/// <summary>
	/// The start point.
	/// </summary>
	Vector<float, N> Start;

	/// <summary>
	/// The end point.
	/// </summary>
	Vector<float, N> End;

	/// <summary>
	/// Initialize new <see cref="Line"/> instance.
	/// </summary>
	constexpr Line()
	{
	}

	/// <summary>
	/// Initialize new <see cref="Line"/> instance.
	/// </summary>
	/// <param name="start"> The start point. </param>
	/// <param name="end"> The end point. </param>
	constexpr Line(const Vector<float, N>& start, const Vector<float, N>& end)
		: Start(start)
		, End(end)
	{
	}

	/// <summary>
	/// Initialize new <see cref="Line"/> instance.
	/// </summary>
	/// <param name="copy"> The initial value. </param>
	constexpr Line(const Line& copy)
		: Start(copy.Start)
		, End(copy.End)
	{
	}

	/// <summary>
	/// Compare nearly equals to target line with epsilon value.
	/// </summary>
	/// <param name="rhs"> The target line. </param>
	/// <param name="epsilon"> The epsilon value. If different of two components is lower than this values, is nearly equals. </param>
	/// <returns> Indicate two lines is nearly equals. </returns>
	inline constexpr bool NearlyEquals(const Line& rhs, float epsilon) const
	{
		return Start.NearlyEquals(rhs.Start, epsilon)
			&& End.NearlyEquals(rhs.End, epsilon);
	}

	/// <summary>
	/// Get simple string represents this line value.
	/// </summary>
	/// <param name="formatArgs"> The formatting args that use to std::format. </param>
	/// <returns> The simple string value. </returns>
	inline std::wstring ToString(std::wstring_view formatArgs = L"") const
	{
		return std::format(L"Start: {}, End: {}", Start.ToString(formatArgs), End.ToString(formatArgs));
	}

	/// <summary>
	/// Get point of interpolate vector linked start to end.
	/// </summary>
	/// <param name="factor"> The interpolate factor. </param>
	/// <returns> The point vector. </returns>
	inline constexpr Vector<float, N> GetPoint(float factor) const
	{
		return Start + GetVector() * factor;
	}

	/// <summary>
	/// Get vector of linked start to end.
	/// </summary>
	/// <returns> The vector. </returns>
	inline constexpr Vector<float, N> GetVector() const
	{
		return End - Start;
	}
};