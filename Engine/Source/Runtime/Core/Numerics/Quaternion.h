// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Vector4.h"
#include <initializer_list>
#include "Vector3.h"
#include "Mathematics/MathEx.h"
#include "Mathematics/Degrees.h"

struct Quaternion : public Vector4
{
	/// <summary>
	/// Initialize new <see cref="Quaternion"/> instance.
	/// </summary>
	inline constexpr Quaternion()
	{
	}

	/// <summary>
	/// Initialize new <see cref="Quaternion"/> instance.
	/// </summary>
	/// <param name="v"> The initialize value. </param>
	inline constexpr Quaternion(float v)
	{
		for (size_t i = 0; i < Num(); ++i)
		{
			Values[i] = v;
		}
	}

	/// <summary>
	/// Initialize new <see cref="Quaternion"/> instance.
	/// </summary>
	/// <param name="x"> The X value. </param>
	/// <param name="y"> The Y value. </param>
	/// <param name="z"> The Z value. </param>
	/// <param name="w"> The Z value. </param>
	inline constexpr Quaternion(float x, float y, float z, float w) : Vector4(x, y, z, w)
	{
	}

	/// <summary>
	/// Initialize new <see cref="Quaternion"/> instance.
	/// </summary>
	/// <param name="initializer"> The initializer to initialize vector values. </param>
	inline constexpr Quaternion(std::initializer_list<float> initializer)
	{
		for (size_t i = 0; i < MathEx::Min(initializer.size(), Num()); ++i)
		{
			Values[i] = initializer.begin()[i];
		}
	}

	/// <summary>
	/// Initialize new <see cref="Quaternion"/> instance.
	/// </summary>
	/// <param name="copy"> The copy vector to initialize vector values. </param>
	inline constexpr Quaternion(const Vector<4>& copy)
	{
		for (size_t i = 0; i < Num(); ++i)
		{
			Values[i] = copy[i];
		}
	}

	/// <summary>
	/// Initialize new <see cref="Quaternion"/> instance.
	/// </summary>
	/// <param name="copy"> The copy vector to initialize vector values. </param>
	/// <param name="fill"> The fill value for placed without initialization. </param>
	template<size_t ON>
	inline constexpr Quaternion(const Vector<ON>& copy, float fill = 0.0f)
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
	/// Get W component.
	/// </summary>
	/// <returns> Reference of W component. </returns>
	inline constexpr const float& W() const
	{
		return Values[3];
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
	/// Get W component.
	/// </summary>
	/// <returns> Reference of W component. </returns>
	inline float& W()
	{
		return Values[3];
	}

	/// <summary>
	/// Rotate vector.
	/// </summary>
	constexpr Vector3 RotateVector(const Vector3& V) const
	{
		Vector3 Q = Swiz<0, 1, 2>();
		Vector3 T = (Q ^ V) * 2.0f;
		return V + (T * W()) + (Q ^ T);
	}

	/// <summary>
	/// Get inversed quaternion.
	/// </summary>
	constexpr Quaternion GetInverse() const
	{
		return Quaternion(-X(), -Y(), -Z(), W());
	}

	/// <summary>
	/// Get identity quaternion.
	/// </summary>
	static inline constexpr Quaternion GetIdentity();

	static Quaternion FromAxisAngle(const Vector3& axis, Degrees angle);
	static Quaternion LookTo(const Vector3& forward, const Vector3& up);
};

inline constexpr Quaternion Quaternion::GetIdentity()
{
	return Quaternion(0, 0, 0, 1.0f);
}