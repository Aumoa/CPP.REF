// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <initializer_list>
#include "Vector.h"
#include "Mathematics/MathEx.h"
#include "Mathematics/Degrees.h"

struct Quaternion : public VectorImpl<Quaternion, 4>
{
	/// <summary>
	/// Initialize new <see cref="Quaternion"/> instance.
	/// </summary>
	inline constexpr Quaternion() : VectorImpl()
	{
	}

	/// <summary>
	/// Initialize new <see cref="Quaternion"/> instance.
	/// </summary>
	/// <param name="v"> The initialize value. </param>
	inline constexpr Quaternion(float v) : VectorImpl(v)
	{
	}

	/// <summary>
	/// Initialize new <see cref="Quaternion"/> instance.
	/// </summary>
	/// <param name="x"> The X value. </param>
	/// <param name="y"> The Y value. </param>
	/// <param name="z"> The Z value. </param>
	/// <param name="w"> The Z value. </param>
	inline constexpr Quaternion(float x, float y, float z, float w) : VectorImpl(x, y, z, w)
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
			Scalars[i] = initializer.begin()[i];
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
			Scalars[i] = copy[i];
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
				Scalars[i] = copy[i];
			}
			else
			{
				Scalars[i] = fill;
			}
		}
	}

	/// <summary>
	/// Rotate vector.
	/// </summary>
	constexpr Vector3 RotateVector(const Vector3& V) const
	{
		Vector3 Q(X, Y, Z);
		Vector3 T = (Q ^ V) * 2.0f;
		return V + (T * W) + (Q ^ T);
	}

	/// <summary>
	/// Get inversed quaternion.
	/// </summary>
	constexpr Quaternion GetInverse() const
	{
		return Quaternion(-X, -Y, -Z, W);
	}

	/// <summary>
	/// Get identity quaternion.
	/// </summary>
	static inline constexpr Quaternion GetIdentity()
	{
		return Quaternion(0, 0, 0, 1.0f);
	}

	CORE_API static Quaternion FromAxisAngle(const Vector3& axis, Degrees angle);
	CORE_API static Quaternion LookTo(const Vector3& forward, const Vector3& up);
};