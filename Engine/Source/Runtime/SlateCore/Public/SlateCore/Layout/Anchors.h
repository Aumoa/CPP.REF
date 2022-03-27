// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Numerics/VectorInterface/Vector.h"

struct Anchors
{
	Vector2 Minimum;
	Vector2 Maximum;

	constexpr Anchors()
	{
	}

	constexpr Anchors(float UniformAnchors)
		: Minimum(UniformAnchors)
		, Maximum(UniformAnchors)
	{
	}

	constexpr Anchors(float Horizontal, float Vertical)
		: Minimum(Horizontal, Vertical)
		, Maximum(Horizontal, Vertical)
	{
	}

	constexpr Anchors(float MinX, float MinY, float MaxX, float MaxY)
		: Minimum(MinX, MinY)
		, Maximum(MaxX, MaxY)
	{
	}

	constexpr Anchors(const Vector2& Minimum, const Vector2& Maximum)
		: Minimum(Minimum)
		, Maximum(Maximum)
	{
	}

	constexpr bool IsStretchedVertical() const
	{
		return Minimum.Y != Maximum.Y;
	}

	constexpr bool IsStretchedHorizontal() const
	{
		return Minimum.X != Maximum.X;
	}

	constexpr bool operator ==(const Anchors& Rhs) const
	{
		return Minimum == Rhs.Minimum
			&& Maximum == Rhs.Maximum;
	}

	constexpr bool operator !=(const Anchors& Rhs) const
	{
		return Minimum != Rhs.Minimum
			|| Maximum != Rhs.Maximum;
	}

public:
	using Type = float;
	using VectorType = Vector2;

	static constexpr size_t Row()
	{
		return 2;
	}

	static constexpr size_t Column()
	{
		return 2;
	}

	constexpr Anchors operator -() const
	{
		return { -Minimum, -Maximum };
	}

	constexpr const Vector2& operator [](size_t N) const
	{
		return N == 0 ? Minimum : Maximum;
	}

	constexpr Vector2& operator [](size_t N)
	{
		return N == 0 ? Minimum : Maximum;
	}

	template<TIsMatrix<float, 2, 2> IMatrix>
	constexpr Anchors& operator =(const IMatrix& M)
	{
		Minimum = M[0];
		Maximum = M[1];
		return *this;
	}

public:
	inline std::wstring ToString(std::wstring_view FormatArgs) const
	{
		return String::Format(L"{{{Minimum={}, Maximum={}}}", Minimum.ToString(FormatArgs), Maximum.ToString(FormatArgs));
	}

	template<TIsMatrix<float, 2, 2> IMatrix>
	inline constexpr bool NearlyEquals(const IMatrix& M, float Epsilon) const
	{
		return Matrix<>::NearlyEquals(*this, M, Epsilon);
	}
};