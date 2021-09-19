// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Mathematics/MathEx.h"
#include "Vector.h"

struct Rect
{
	float Left = 0;
	float Top = 0;
	float Right = 0;
	float Bottom = 0;

	constexpr Rect()
	{
	}

	constexpr Rect(float left, float top, float right, float bottom)
		: Left(left)
		, Top(top)
		, Right(right)
		, Bottom(bottom)
	{
	}

	constexpr Rect(const Vector2& leftTop, const Vector2& rightBottom)
		: Left(leftTop.X), Top(leftTop.Y)
		, Right(rightBottom.X), Bottom(rightBottom.Y)
	{
	}

	constexpr Rect(const Vector<float, 4>& v) : Rect(v[0], v[1], v[2], v[3])
	{
	}

	std::wstring ToString(std::wstring_view formatArgs = L"") const
	{
		return std::format(L"{{LT: {}, RB: {}, [{} * {}]}}", GetLeftTop().ToString(formatArgs), GetRightBottom().ToString(formatArgs), GetWidth(), GetHeight());
	}

	constexpr bool NearlyEquals(const Rect& rhs, float epsilon = MathEx::SmallNumber) const
	{
		return MathEx::Abs(Left - rhs.Left) <= epsilon
			&& MathEx::Abs(Top - rhs.Top) <= epsilon
			&& MathEx::Abs(Right - rhs.Right) <= epsilon
			&& MathEx::Abs(Bottom - rhs.Bottom) <= epsilon;
	}

	constexpr Vector2 GetLeftTop() const
	{
		return Vector2(Left, Top);
	}

	constexpr Vector2 GetRightBottom() const
	{
		return Vector2(Right, Bottom);
	}

	constexpr float GetWidth() const
	{
		return MathEx::Abs(Right - Left);
	}

	constexpr float GetHeight() const
	{
		return MathEx::Abs(Bottom - Top);
	}

	constexpr bool PtInRect(const Vector2& point) const
	{
		return Left <= point.X && point.X <= Right
			&& Top <= point.Y && point.Y <= Bottom;
	}
};