// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Numerics/VectorInterface/Vector.h"

struct Scale2D
{
	float X;
	float Y;

	constexpr Scale2D(float X, float Y) : X(X), Y(Y)
	{
	}

	constexpr Scale2D(const Scale2D& S) : X(S.X), Y(S.Y)
	{
	}

	template<TIsVector<float, 2> IScale>
	constexpr Scale2D(const IScale& S) : X(S[0]), Y(S[1])
	{
	}

	constexpr Scale2D& operator =(const Scale2D& S)
	{
		X = S.X;
		Y = S.Y;
		return *this;
	}

public:
	using Type = float;

	constexpr Scale2D(float Uniform = 0) : X(Uniform), Y(Uniform)
	{
	}

	static constexpr size_t Size()
	{
		return 2;
	}

	constexpr Scale2D operator -() const
	{
		return Scale2D(-X, -Y);
	}

	constexpr const float& operator [](size_t N) const
	{
		return N == 0 ? X : Y;
	}

	constexpr float& operator [](size_t N)
	{
		return N == 0 ? X : Y;
	}

	template<TIsVector<float, 2> IVector>
	constexpr Scale2D& operator =(const IVector& V)
	{
		X = V[0];
		Y = V[1];
		return *this;
	}

public:
	template<TIsVector<float, 2> IScale>
	static constexpr Scale2D Inverse(const IScale& S)
	{
		return 1.0f / S;
	}

	constexpr Scale2D Inverse() const
	{
		return Inverse(*this);
	}

	template<TIsVector<float, 2> IScaleL, TIsVector<float, 2> IScaleR>
	static constexpr Scale2D Concatenate(const IScaleL& SL, const IScaleR& SR)
	{
		return SL * SR;
	}

	template<TIsVector<float, 2> IScale>
	constexpr Scale2D Concatenate(const IScale& S) const
	{
		return Concatenate(*this, S);
	}

	static constexpr Scale2D Identity()
	{
		return Scale2D(1.0f);
	}

	template<TIsVector<float, 2> IScale, TIsVector<float, 2> IPoint>
	static constexpr IPoint TransformPoint(const IScale& S, const IPoint& P)
	{
		return S * P;
	}

	template<TIsVector<float, 2> IPoint>
	constexpr IPoint TransformPoint(const IPoint& P) const
	{
		return TransformPoint(*this, P);
	}

	template<TIsVector<float, 2> IScale, TIsVector<float, 2> IVector>
	static constexpr IVector TransformVector(const IScale& S, const IVector& V)
	{
		return S * V;
	}

	template<TIsVector<float, 2> IVector>
	constexpr IVector TransformVector(const IVector& V) const
	{
		return TransformVector(*this, V);
	}

public:
	String ToString(String formatArgs) const
	{
		return Vector<>::ToString(*this, formatArgs);
	}

	constexpr bool NearlyEquals(const Scale2D& S, float Epsilon) const
	{
		return Vector<>::NearlyEquals(*this, S, Epsilon);
	}
};