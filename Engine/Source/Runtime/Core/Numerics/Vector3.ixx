// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:Vector3;

import std.core;
import :Vector;

using namespace std;

/// <summary>
/// Represent a vector with 3 floating point values.
/// </summary>
export struct Vector3 : public Vector<3>
{
	/// <summary>
	/// Initialize new <see cref="Vector3"/> instance.
	/// </summary>
	inline constexpr Vector3()
	{
	}

	/// <summary>
	/// Initialize new <see cref="Vector3"/> instance.
	/// </summary>
	/// <param name="v"> The initialize value. </param>
	inline constexpr Vector3(float v)
	{
		for (size_t i = 0; i < Num(); ++i)
		{
			Values[i] = v;
		}
	}

	/// <summary>
	/// Initialize new <see cref="Vector3"/> instance.
	/// </summary>
	/// <param name="x"> The X value. </param>
	/// <param name="y"> The Y value. </param>
	/// <param name="z"> The Z value. </param>
	inline constexpr Vector3(float x, float y, float z) : Vector{ x, y, z }
	{
	}

	/// <summary>
	/// Initialize new <see cref="Vector3"/> instance.
	/// </summary>
	/// <param name="initializer"> The initializer to initialize vector values. </param>
	inline constexpr Vector3(initializer_list<float> initializer)
	{
		for (size_t i = 0; i < MathEx::Min(initializer.size(), (size_t)Num()); ++i)
		{
			Values[i] = initializer.begin()[i];
		}
	}

	/// <summary>
	/// Initialize new <see cref="Vector3"/> instance.
	/// </summary>
	/// <param name="copy"> The copy vector to initialize vector values. </param>
	inline constexpr Vector3(const Vector<3>& copy)
	{
		for (size_t i = 0; i < Num(); ++i)
		{
			Values[i] = copy[i];
		}
	}

	/// <summary>
	/// Initialize new <see cref="Vector3"/> instance.
	/// </summary>
	/// <param name="copy"> The copy vector to initialize vector values. </param>
	/// <param name="fill"> The fill value for placed without initialization. </param>
	template<size_t ON>
	inline constexpr Vector3(const Vector<ON>& copy, float fill = 0.0f)
	{
		for (size_t i = 0; i < Num(); ++i)
		{
			if (i < ON)
			{
				Values[i] = copy[i];
			}
			else
			{
				Values[i] = fill;
			}
		}
	}

	/// <summary>
	/// Calculate cross product.
	/// </summary>
	/// <param name="lhs"> The first value. </param>
	/// <param name="rhs"> The second value. </param>
	/// <returns> The vector of result of cross product. </returns>
	static Vector3 CrossProduct(const Vector3& lhs, const Vector3& rhs)
	{
		return
		{
			lhs.Y() * rhs.Z() - lhs.Z() * rhs.Y(),
			lhs.Z() * rhs.X() - lhs.X() * rhs.Z(),
			lhs.X() * rhs.Y() - lhs.Y() * rhs.X()
		};
	}

	/// <summary>
	/// Get X component.
	/// </summary>
	/// <returns> Reference of X component. </returns>
	inline constexpr const float& X() const
	{
		return Values[0];
	}

	/// <summary>
	/// Get Y component.
	/// </summary>
	/// <returns> Reference of Y component. </returns>
	inline constexpr const float& Y() const
	{
		return Values[1];
	}

	/// <summary>
	/// Get Z component.
	/// </summary>
	/// <returns> Reference of Z component. </returns>
	inline constexpr const float& Z() const
	{
		return Values[2];
	}

	/// <summary>
	/// Get X component.
	/// </summary>
	/// <returns> Reference of X component. </returns>
	inline float& X()
	{
		return Values[0];
	}

	/// <summary>
	/// Get Y component.
	/// </summary>
	/// <returns> Reference of Y component. </returns>
	inline float& Y()
	{
		return Values[1];
	}

	/// <summary>
	/// Get Z component.
	/// </summary>
	/// <returns> Reference of Z component. </returns>
	inline float& Z()
	{
		return Values[2];
	}
};