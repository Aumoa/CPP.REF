// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Numerics/VectorInterface/Vector.h"

namespace libty::inline CoreNumerics
{
	struct Translate2D
	{
		float X;
		float Y;

		constexpr Translate2D(float X, float Y) : X(X), Y(Y)
		{
		}

		constexpr Translate2D(const Translate2D& T) : X(T.X), Y(T.Y)
		{
		}

		template<TIsVector<float, 2> IVector>
		constexpr Translate2D(const IVector& V) : X(V[0]), Y(V[1])
		{
		}

		constexpr Translate2D& operator =(const Translate2D& T)
		{
			X = T.X;
			Y = T.Y;
			return *this;
		}

	public:
		using Type = float;

		constexpr Translate2D(float Uniform = 0) : X(Uniform), Y(Uniform)
		{
		}

		static constexpr size_t Size()
		{
			return 2;
		}

		constexpr Translate2D operator -() const
		{
			return Translate2D(-X, -Y);
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
		constexpr Translate2D& operator =(const IVector& V)
		{
			X = V[0];
			Y = V[1];
			return *this;
		}

	public:
		template<TIsVector<float, 2> ITranslate>
		static constexpr Translate2D Inverse(const ITranslate& T)
		{
			return -T;
		}

		constexpr Translate2D Inverse() const
		{
			return Inverse(*this);
		}

		template<TIsVector<float, 2> ITranslateL, TIsVector<float, 2> ITranslateR>
		static constexpr Translate2D Concatenate(const ITranslateL& TL, const ITranslateR& TR)
		{
			return TL + TR;
		}

		template<TIsVector<float, 2> ITranslate>
		constexpr Translate2D Concatenate(const ITranslate& T) const
		{
			return Concatenate(*this, T);
		}

		static constexpr Translate2D Identity()
		{
			return Translate2D(0);
		}

		template<TIsVector<float, 2> ITranslate, TIsVector<float, 2> IVector>
		constexpr static IVector TransformPoint(const ITranslate& T, const IVector& P)
		{
			return P + T;
		}

		template<TIsVector<float, 2> IVector>
		constexpr IVector TransformPoint(const IVector& P) const
		{
			return TransformPoint(*this, P);
		}

		template<TIsVector<float, 2> ITranslate, TIsVector<float, 2> IVector>
		constexpr static IVector TransformVector(const ITranslate& T, const IVector& V)
		{
			return V;
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

		constexpr bool NearlyEquals(const Translate2D& T, float Epsilon) const
		{
			return Vector<>::NearlyEquals(*this, T, Epsilon);
		}

		constexpr Translate2D Round() const
		{
			return { MathEx::Round(X), MathEx::Round(Y) };
		}
	};
}