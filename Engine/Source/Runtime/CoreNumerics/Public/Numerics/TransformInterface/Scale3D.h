// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Numerics/VectorInterface/Vector.h"

namespace libty::inline CoreNumerics::inline TransformInterface
{
	struct Scale3D
	{
		using IsScale = int;

		float X;
		float Y;
		float Z;

		constexpr Scale3D(float X, float Y, float Z) : X(X), Y(Y), Z(Z)
		{
		}

		constexpr Scale3D(const Scale3D& S) : X(S.X), Y(S.Y), Z(S.Z)
		{
		}

		template<TIsVector<float, 3> IScale>
		constexpr Scale3D(const IScale& S) : X(S[0]), Y(S[1]), Z(S[2])
		{
		}

		constexpr Scale3D& operator =(const Scale3D& S)
		{
			X = S.X;
			Y = S.Y;
			Z = S.Z;
			return *this;
		}

	public:
		using Type = float;

		constexpr Scale3D(float Uniform = 0) : X(Uniform), Y(Uniform), Z(Uniform)
		{
		}

		static constexpr size_t Size()
		{
			return 3;
		}

		constexpr Scale3D operator -() const
		{
			return Scale3D(-X, -Y, -Z);
		}

		constexpr const float& operator [](size_t N) const
		{
			switch (N)
			{
			case 0:
				return X;
			case 1:
				return Y;
			case 2:
			default:
				return Z;
			}
		}

		constexpr float& operator [](size_t N)
		{
			switch (N)
			{
			case 0:
				return X;
			case 1:
				return Y;
			case 2:
			default:
				return Z;
			}
		}

		template<TIsVector<float, 3> IVector>
		constexpr Scale3D& operator =(const IVector& V)
		{
			X = V[0];
			Y = V[1];
			Z = V[2];
			return *this;
		}

	public:
		template<TIsVector<float, 3> IScale>
		static constexpr Scale3D Inverse(const IScale& S)
		{
			return 1.0f / S;
		}

		constexpr Scale3D Inverse() const
		{
			return Inverse(*this);
		}

		template<TIsVector<float, 3> IScaleL, TIsVector<float, 3> IScaleR>
		static constexpr Scale3D Concatenate(const IScaleL& SL, const IScaleR& SR)
		{
			return SL * SR;
		}

		template<TIsVector<float, 3> IScale>
		constexpr Scale3D Concatenate(const IScale& S) const
		{
			return Concatenate(*this, S);
		}

		static constexpr Scale3D Identity()
		{
			return Scale3D(1.0f);
		}

		template<TIsVector<float, 3> IScale, TIsVector<float, 3> IPoint>
		static constexpr IPoint TransformPoint(const IScale& S, const IPoint& P)
		{
			return S * P;
		}

		template<TIsVector<float, 3> IPoint>
		constexpr IPoint TransformPoint(const IPoint& P) const
		{
			return TransformPoint(*this, P);
		}

		template<TIsVector<float, 3> IScale, TIsVector<float, 3> IVector>
		static constexpr IVector TransformVector(const IScale& S, const IVector& V)
		{
			return S * V;
		}

		template<TIsVector<float, 3> IVector>
		constexpr IVector TransformVector(const IVector& V) const
		{
			return TransformVector(*this, V);
		}

	public:
		std::wstring ToString(std::wstring_view FormatArgs) const
		{
			return Vector<>::ToString(*this, FormatArgs);
		}

		constexpr bool NearlyEquals(const Scale3D& S, float Epsilon) const
		{
			return Vector<>::NearlyEquals(*this, S, Epsilon);
		}
	};
}