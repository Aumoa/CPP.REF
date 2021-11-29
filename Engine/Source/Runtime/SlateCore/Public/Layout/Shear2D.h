// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

struct Shear2D
{
	constexpr Shear2D()
	{
	}

	constexpr Shear2D(float ShearX, float ShearY) : Shear(ShearX, ShearY)
	{
	}

	constexpr Shear2D(const Vector2& Shear) : Shear(Shear)
	{
	}

	std::wstring ToString(std::wstring_view InFormatArgs = L"") const
	{
		return std::format(L"Shear: {}", Shear.ToString(InFormatArgs));
	}

	static Shear2D FromShearAngles(const Vector2& Angles)
	{
		// Compute the M (Shear Slot) = CoTan(90 - SlopeAngle)

		// 0 is a special case because Tan(90) == infinity
		float ShearX = Angles.X == 0 ? 0 : (1.0f / MathEx::Tan(Degrees(90.0f - MathEx::Clamp(Angles.X, -89.0f, 89.0f)).ToRadians()));
		float ShearY = Angles.Y == 0 ? 0 : (1.0f / MathEx::Tan(Degrees(90.0f - MathEx::Clamp(Angles.Y, -89.0f, 89.0f)).ToRadians()));

		return Shear2D(ShearX, ShearY);
	}

	constexpr Vector2 TransformPoint(const Vector2& Point)
	{
		return
		{
			Point[0] + Point[1] * Shear.X,
			Point[1] + Point[0] * Shear.Y
		};
	}

	constexpr Vector2 TransformVector(const Vector2& Vector)
	{
		return TransformPoint(Vector);
	}

	constexpr Matrix3x2 Concatenate(const Shear2D& Rhs) const
	{
		float XXA = Shear.X;
		float YYA = Shear.Y;
		float XXB = Rhs.Shear.X;
		float YYB = Rhs.Shear.Y;
		return Matrix3x2
		{
			1 + YYA * XXB, YYB * YYA,
			XXA + XXB, XXA * XXB + 1,
			0, 0
		};
	}

	constexpr const Vector2& GetVector() const
	{
		return Shear;
	}

	constexpr Matrix3x2 Inverse() const
	{
		float invDet = 1.0f / (1.0f - Shear.X * Shear.Y);
		return Matrix3x2
		{
			invDet, -Shear.Y * invDet,
			-Shear.X * invDet, invDet,
			0, 0
		};
	}

	constexpr bool operator ==(const Shear2D& Rhs) const
	{
		return Shear == Rhs.Shear;
	}

	constexpr bool operator !=(const Shear2D& Rhs) const
	{
		return Shear != Rhs.Shear;
	}

	static constexpr Shear2D Identity()
	{
		return Shear2D(0.0f);
	}

private:
	Vector2 Shear;
};