// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

/// <summary>
/// Represents aligned of standard basis box geometry.
/// </summary>
template<size_t N>
struct AxisAlignedCube
{
	/// <summary>
	/// Minimum aligned offset.
	/// </summary>
	Vector<N> Min;

	/// <summary>
	/// Maximum aligned offset.
	/// </summary>
	Vector<N> Max;

	/// <summary>
	/// Initialize new <see cref="AxisAlignedCube"/> instance.
	/// </summary>
	constexpr AxisAlignedCube()
	{
	}

	/// <summary>
	/// Initialize new <see cref="AxisAlignedCube"/> instance.
	/// </summary>
	/// <param name="min"> The minimum aligned offset. </param>
	/// <param name="max"> The maximum aligned offset. </param>
	constexpr AxisAlignedCube(const Vector<N>& min, const Vector<N>& max) : Min(min), Max(max)
	{
	}

	/// <summary>
	/// Initialize new <see cref="AxisAlignedCube"/> instance.
	/// </summary>
	/// <param name="copy"> The initial value. </param>
	constexpr AxisAlignedCube(const AxisAlignedCube& copy) : Min(copy.Min), Max(copy.Max)
	{
	}

	/// <summary>
	/// Compare nearly equals to target cube with epsilon value.
	/// </summary>
	/// <param name="rhs"> The target cube. </param>
	/// <param name="epsilon"> The epsilon value. If different of two components is lower than this values, is nearly equals. </param>
	/// <returns> Indicate two cubes is nearly equals. </returns>
	constexpr bool NearlyEquals(const AxisAlignedCube& rhs, float epsilon) const
	{
		return Min.NearlyEquals(rhs.Min, epsilon)
			&& Max.NearlyEquals(rhs.Max, epsilon);
	}

	/// <summary>
	/// Get simple string represents this cube value.
	/// </summary>
	/// <param name="formatArgs"> The formatting args that use to std::format. </param>
	/// <returns> The simple string value. </returns>
	wstring ToString(wstring_view formatArgs = L"") const
	{
		return format(L"Min: {}, Max: {}", Min.ToString(formatArgs), Max.ToString(formatArgs));
	}

	/// <summary>
	/// Get center point.
	/// </summary>
	/// <returns> The center point vector. </returns>
	constexpr Vector<N> GetCenter() const
	{
		return (Min + Max) * 0.5f;
	}

	/// <summary>
	/// Get extent vector.
	/// </summary>
	/// <returns> The extent vector. </returns>
	constexpr Vector<N> GetExtent() const
	{
		return (Max - Min) * 0.5f;
	}
};