// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Mathematics/MathEx.h"
#include "Vector.h"

template<class T>
struct RectT
{
	T Left = 0;
	T Top = 0;
	T Right = 0;
	T Bottom = 0;

	constexpr RectT()
	{
	}

	constexpr RectT(T left, T top, T right, T bottom)
		: Left(left)
		, Top(top)
		, Right(right)
		, Bottom(bottom)
	{
	}

	constexpr RectT(const Vector<T, 2>& leftTop, const Vector<T, 2>& rightBottom)
		: Left(leftTop.X), Top(leftTop.Y)
		, Right(rightBottom.X), Bottom(rightBottom.Y)
	{
	}

	constexpr RectT(const Vector<T, 4>& v) : RectT(v[0], v[1], v[2], v[3])
	{
	}

	std::wstring ToString(std::wstring_view formatArgs = L"") const
	{
		return std::format(L"{{LT: {}, RB: {}, [{} * {}]}}", GetLeftTop().ToString(formatArgs), GetRightBottom().ToString(formatArgs), GetWidth(), GetHeight());
	}

	constexpr bool NearlyEquals(const RectT& rhs, T epsilon = MathEx::SmallNumber) const requires requires
	{
		{ T(MathEx::SmallNumber) < T(1) };
	}
	{
		return MathEx::Abs(Left - rhs.Left) <= epsilon
			&& MathEx::Abs(Top - rhs.Top) <= epsilon
			&& MathEx::Abs(Right - rhs.Right) <= epsilon
			&& MathEx::Abs(Bottom - rhs.Bottom) <= epsilon;
	}

	constexpr Vector<T, 2> GetLeftTop() const
	{
		return Vector<T, 2>(Left, Top);
	}

	constexpr Vector<T, 2> GetRightBottom() const
	{
		return Vector<T, 2>(Right, Bottom);
	}

	constexpr T GetWidth() const
	{
		return MathEx::Abs(Right - Left);
	}

	constexpr T GetHeight() const
	{
		return MathEx::Abs(Bottom - Top);
	}

	constexpr bool PtInRect(const Vector<T, 2>& point) const
	{
		return Left <= point.X && point.X <= Right
			&& Top <= point.Y && point.Y <= Bottom;
	}
};

using Rect = RectT<float>;
using RectN = RectT<int32>;