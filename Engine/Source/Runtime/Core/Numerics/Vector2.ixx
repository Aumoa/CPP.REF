// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:Vector2;

import std.core;
import :Vector;

using namespace std;

/// <summary>
/// Represent a vector with 2 floating point values.
/// </summary>
export struct Vector2 : public Vector<2>
{
	/// <summary>
	/// Initialize new <see cref="Vector2"/> instance.
	/// </summary>
	inline constexpr Vector2()
	{
	}

	/// <summary>
	/// Initialize new <see cref="Vector2"/> instance.
	/// </summary>
	/// <param name="v"> The initialize value. </param>
	inline constexpr Vector2(float v)
	{
		for (size_t i = 0; i < Num(); ++i)
		{
			Values[i] = v;
		}
	}

	/// <summary>
	/// Initialize new <see cref="Vector2"/> instance.
	/// </summary>
	/// <param name="x"> The X value. </param>
	/// <param name="y"> The Y value. </param>
	inline constexpr Vector2(float x, float y) : Vector{ x, y }
	{
	}

	/// <summary>
	/// Initialize new <see cref="Vector2"/> instance.
	/// </summary>
	/// <param name="initializer"> The initializer to initialize vector values. </param>
	inline constexpr Vector2(initializer_list<float> initializer)
	{
		for (size_t i = 0; i < MathEx::Min(initializer.size(), Num()); ++i)
		{
			Values[i] = initializer.begin()[i];
		}
	}

	/// <summary>
	/// Initialize new <see cref="Vector2"/> instance.
	/// </summary>
	/// <param name="copy"> The copy vector to initialize vector values. </param>
	inline constexpr Vector2(const Vector<2>& copy)
	{
		for (size_t i = 0; i < Num(); ++i)
		{
			Values[i] = copy[i];
		}
	}

	/// <summary>
	/// Initialize new <see cref="Vector2"/> instance.
	/// </summary>
	/// <param name="copy"> The copy vector to initialize vector values. </param>
	/// <param name="fill"> The fill value for placed without initialization. </param>
	template<size_t ON>
	inline constexpr Vector2(const Vector<ON>& copy, float fill = 0.0f)
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
	static Vector2 CrossProduct(const Vector2& lhs, const Vector2& rhs)
	{
		return
		{
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
};