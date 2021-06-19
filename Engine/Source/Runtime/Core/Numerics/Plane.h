// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

/// <summary>
/// Represents a plane that defined as normal and distance.
/// </summary>
struct Plane
{
	/// <summary>
	/// The normal of plane.
	/// </summary>
	Vector3 Normal;

	/// <summary>
	/// The distance from zero point.
	/// </summary>
	float Distance = 0;

	/// <summary>
	/// Initialize new <see cref="Plane"/> instance.
	/// </summary>
	constexpr Plane()
	{
	}

	/// <summary>
	/// Initialize new <see cref="Plane"/> instance.
	/// </summary>
	/// <param name="normal"> The normal of plane. </param>
	/// <param name="distance"> The distance from zero point. </param>
	constexpr Plane(const Vector3& normal, float distance) : Normal(normal), Distance(distance)
	{
	}

	/// <summary>
	/// Compare nearly equals to target plane with epsilon value.
	/// </summary>
	/// <param name="rhs"> The target plane. </param>
	/// <param name="epsilon"> The epsilon value. If different of two components is lower than this values, is nearly equals. </param>
	/// <returns> Indicate two planes is nearly equals. </returns>
	constexpr bool NearlyEquals(const Plane& rhs, float epsilon) const
	{
		return Normal.NearlyEquals(rhs.Normal, epsilon)
			&& MathEx::Abs(Distance - rhs.Distance) <= epsilon;
	}

	/// <summary>
	/// Get simple string represents this plane value.
	/// </summary>
	/// <param name="formatArgs"> The formatting args that use to std::format. </param>
	/// <returns> The simple string value. </returns>
	wstring ToString(wstring_view formatArgs = L"") const
	{
		wstring placeholder = StringUtils::GetPlaceholder(formatArgs);
		return format(L"Normal: {}, Distance: {}", Normal.ToString(formatArgs), format(placeholder, Distance));
	}

	/// <summary>
	/// Get normalized plane.
	/// </summary>
	/// <returns> The normalized plane. </returns>
	Plane GetNormal() const
	{
		float sq = Normal.GetLengthSq();
		float rt = MathEx::InvSqrt(sq);

		return Plane(Normal * rt, Distance * rt);
	}

	/// <summary>
	/// Get dot product with coordinate.
	/// </summary>
	/// <param name="plane"> The plane. </param>
	/// <param name="coord"> The coordinate. </param>
	/// <returns> The dot producted value. </returns>
	static float DotCoord(const Plane& plane, const Vector3& coord)
	{
		return plane[0] * coord[0] + plane[1] * coord[1] + plane[2] * coord[2] + plane[3] * 1.0f;
	}

private:
	const float& operator [](size_t index) const
	{
		return (&Normal.Values[0])[index];
	}
};