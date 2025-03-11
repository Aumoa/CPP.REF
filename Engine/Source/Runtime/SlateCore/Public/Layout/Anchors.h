// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"

struct NAnchors
{
	Vector2 Minimum;
	Vector2 Maximum;

	constexpr NAnchors()
	{
	}

	constexpr NAnchors(float UniformAnchors)
		: Minimum(UniformAnchors)
		, Maximum(UniformAnchors)
	{
	}

	constexpr NAnchors(float Horizontal, float Vertical)
		: Minimum(Horizontal, Vertical)
		, Maximum(Horizontal, Vertical)
	{
	}

	constexpr NAnchors(float MinX, float MinY, float MaxX, float MaxY)
		: Minimum(MinX, MinY)
		, Maximum(MaxX, MaxY)
	{
	}

	constexpr NAnchors(const Vector2& Minimum, const Vector2& Maximum)
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

	constexpr bool operator ==(const NAnchors& Rhs) const
	{
		return Minimum == Rhs.Minimum
			&& Maximum == Rhs.Maximum;
	}

	constexpr bool operator !=(const NAnchors& Rhs) const
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

	constexpr NAnchors operator -() const
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
	constexpr NAnchors& operator =(const IMatrix& M)
	{
		Minimum = M[0];
		Maximum = M[1];
		return *this;
	}

public:
	inline String ToString() const
	{
		return String::Format(TEXT("{{{Minimum={}, Maximum={}}}"), Minimum.ToString(), Maximum.ToString());
	}

	template<TIsMatrix<float, 2, 2> IMatrix>
	inline constexpr bool NearlyEquals(const IMatrix& M, float Epsilon) const
	{
		return Matrix<>::NearlyEquals(*this, M, Epsilon);
	}
};