// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"

struct Shear2D
{
	constexpr Shear2D()
	{
	}

	constexpr Shear2D(float shearX, float shearY) : Shear(shearX, shearY)
	{
	}

	constexpr Shear2D(const Vector2& shear) : Shear(shear)
	{
	}

	std::wstring ToString(std::wstring_view formatArgs = L"") const
	{
		return std::format(L"Shear: {}", Shear.ToString(formatArgs));
	}

	static Shear2D FromShearAngles(const Vector2& angles)
	{
		// Compute the M (Shear Slot) = CoTan(90 - SlopeAngle)

		// 0 is a special case because Tan(90) == infinity
		float shearX = angles.X() == 0 ? 0 : (1.0f / MathEx::Tan(Degrees(90.0f - MathEx::Clamp(angles.X(), -89.0f, 89.0f)).ToRadians()));
		float shearY = angles.Y() == 0 ? 0 : (1.0f / MathEx::Tan(Degrees(90.0f - MathEx::Clamp(angles.Y(), -89.0f, 89.0f)).ToRadians()));

		return Shear2D(shearX, shearY);
	}

	constexpr Vector2 TransformPoint(const Vector2& point)
	{
		return
		{
			point[0] + point[1] * Shear.X(),
			point[1] + point[0] * Shear.Y()
		};
	}

	constexpr Vector2 TransformVector(const Vector2& vector)
	{
		return TransformPoint(vector);
	}

	constexpr Matrix3x2 Concatenate(const Shear2D& rhs) const
	{
		float XXA = Shear.X();
		float YYA = Shear.Y();
		float XXB = rhs.Shear.X();
		float YYB = rhs.Shear.Y();
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
		float invDet = 1.0f / (1.0f - Shear.X() * Shear.Y());
		return Matrix3x2
		{
			invDet, -Shear.Y() * invDet,
			-Shear.X() * invDet, invDet,
			0, 0
		};
	}

	constexpr bool operator ==(const Shear2D& rhs) const
	{
		return Shear == rhs.Shear;
	}

	constexpr bool operator !=(const Shear2D& rhs) const
	{
		return Shear != rhs.Shear;
	}

	static constexpr Shear2D Identity()
	{
		return Shear2D(0.0f);
	}

private:
	Vector2 Shear;
};