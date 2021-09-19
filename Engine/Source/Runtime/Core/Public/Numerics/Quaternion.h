// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <initializer_list>
#include "Vector.h"
#include "Mathematics/MathEx.h"
#include "Mathematics/Degrees.h"

struct Quaternion : public VectorImpl<float, Quaternion, 4>
{
	/// <summary>
	/// Initialize new <see cref="Quaternion"/> instance.
	/// </summary>
	template<class... TArgs> requires std::constructible_from<VectorImpl<float, Quaternion, 4>, TArgs...>
	inline constexpr Quaternion(TArgs&&... args) : VectorImpl(std::forward<TArgs>(args)...)
	{
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
		return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
	}

	CORE_API static Quaternion FromAxisAngle(const Vector3& axis, Degrees angle);
	CORE_API static Quaternion LookTo(const Vector3& forward, const Vector3& up);
};