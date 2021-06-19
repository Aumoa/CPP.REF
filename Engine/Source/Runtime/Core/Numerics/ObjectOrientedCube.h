// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

/// <summary>
/// Represents object oriented cube.
/// </summary>
struct ObjectOrientedCube
{
	/// <summary>
	/// The center location.
	/// </summary>
	Vector3 Center;

	/// <summary>
	/// The extent range.
	/// </summary>
	Vector3 Extent;

	/// <summary>
	/// The rotation quaternion.
	/// </summary>
	Quaternion Rotation;

	/// <summary>
	/// Initialize new <see cref="ObjectOrientedCube"/> instance.
	/// </summary>
	constexpr ObjectOrientedCube()
	{
	}

	/// <summary>
	/// Initialize new <see cref="ObjectOrientedCube"/> instance.
	/// </summary>
	/// <param name="center"> The center location. </param>
	/// <param name="extent"> The extent range. </param>
	/// <param name="rotation"> The rotation quaternion. </param>
	constexpr ObjectOrientedCube(const Vector3& center, const Vector3& extent, const Quaternion& rotation)
		: Center(center)
		, Extent(extent)
		, Rotation(rotation)
	{
	}

	/// <summary>
	/// Compare nearly equals to target cube with epsilon value.
	/// </summary>
	/// <param name="rhs"> The target cube. </param>
	/// <param name="epsilon"> The epsilon value. If different of two components is lower than this values, is nearly equals. </param>
	/// <returns> Indicate two cubes is nearly equals. </returns>
	constexpr bool NearlyEquals(const ObjectOrientedCube& rhs, float epsilon) const
	{
		return Center.NearlyEquals(rhs.Center, epsilon)
			&& Extent.NearlyEquals(rhs.Extent, epsilon)
			&& Rotation.NearlyEquals(rhs.Rotation, epsilon);
	}

	/// <summary>
	/// Get simple string represents this cube value.
	/// </summary>
	/// <param name="formatArgs"> The formatting args that use to std::format. </param>
	/// <returns> The simple string value. </returns>
	wstring ToString(wstring_view formatArgs = L"") const
	{
		return format(L"Center: {}, Extent: {}, Rotation{}", Center.ToString(formatArgs), Extent.ToString(formatArgs), Rotation.ToString(formatArgs));
	}
};