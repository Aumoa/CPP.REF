// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

/// <summary>
/// Represents sphere geometry that contains center and radius.
/// </summary>
template<size_t N>
struct Sphere
{
	/// <summary>
	/// The center location.
	/// </summary>
	Vector<N> Center;

	/// <summary>
	/// The sphere radius.
	/// </summary>
	float Radius = 0;

	/// <summary>
	/// Initialize new <see cref="Sphere"/> instance.
	/// </summary>
	constexpr Sphere()
	{
	}

	/// <summary>
	/// Initialize new <see cref="Sphere"/> instance.
	/// </summary>
	/// <param name="center"> The center location. </param>
	/// <param name="radius"> The sphere radius. </param>
	constexpr Sphere(const Vector<N>& center, float radius) : Center(center), Radius(radius)
	{
	}

	/// <summary>
	/// Initialize new <see cref="Sphere"/> instance.
	/// </summary>
	/// <param name="sphere"> The initial value. </param>
	constexpr Sphere(const Sphere& sphere) : Center(sphere.Center), Radius(sphere.Radius)
	{
	}

	/// <summary>
	/// Compare nearly equals to target sphere with epsilon value.
	/// </summary>
	/// <param name="rhs"> The target sphere. </param>
	/// <param name="epsilon"> The epsilon value. If different of two components is lower than this values, is nearly equals. </param>
	/// <returns> Indicate two spheres is nearly equals. </returns>
	constexpr bool NearlyEquals(const Sphere& rhs, float epsilon) const
	{
		return Center.NearlyEquals(rhs.Center, epsilon)
			&& MathEx::Abs(Radius - rhs.Radius) <= epsilon;
	}

	/// <summary>
	/// Get simple string represents this sphere value.
	/// </summary>
	/// <param name="formatArgs"> The formatting args that use to std::format. </param>
	/// <returns> The simple string value. </returns>
	wstring ToString(wstring_view formatArgs = L"") const
	{
		wstring placeholder = StringUtils::GetPlaceholder(formatArgs);
		return format(L"Center: {}, Radius: {}", Center.ToString(formatArgs), format(placeholder, Radius));
	}
};