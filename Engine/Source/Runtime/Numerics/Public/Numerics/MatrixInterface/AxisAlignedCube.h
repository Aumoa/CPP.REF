// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"

namespace Ayla
{
	template<class T, size_t N = 0>
	struct AxisAlignedCube
	{
		Vector<T, N> Min;
		Vector<T, N> Max;

	public:
		using Type = T;
		using VectorType = Vector<T, N>;

		constexpr AxisAlignedCube(const VectorType& S = VectorType{}) : Min(S), Max(S)
		{
		}

		static constexpr size_t Row()
		{
			return 2;
		}

		static constexpr size_t Column()
		{
			return N;
		}

		constexpr AxisAlignedCube operator -() const
		{
			return AxisAlignedCube(-Min, -Max);
		}

		constexpr const VectorType& operator [](size_t I) const
		{
			return I == 0 ? Min : Max;
		}

		constexpr VectorType& operator [](size_t I)
		{
			return I == 0 ? Min : Max;
		}

		template<TIsMatrix<T, 2, N> IMatrix>
		constexpr AxisAlignedCube& operator =(const IMatrix& M)
		{
			Min = M[0];
			Max = M[1];
			return *this;
		}

		static constexpr AxisAlignedCube Identity()
		{
			AxisAlignedCube R(0.0f);
			R[0][0] = 1.0f;
			if constexpr (N > 1)
			{
				R[1][1] = 1.0;
			}
			return R;
		}

	public:
		constexpr AxisAlignedCube(const VectorType& Min, const VectorType& Max) : Min(Min), Max(Max)
		{
		}

		constexpr AxisAlignedCube(const AxisAlignedCube& AA) : Min(AA.Min), Max(AA.Max)
		{
		}

		template<TIsMatrixTyped<T> IMatrix>
		constexpr AxisAlignedCube(const IMatrix& M) requires (IMatrix::Row() >= 2)
			: Min(M[0]), Max(M[1])
		{
		}

		constexpr AxisAlignedCube& operator =(const AxisAlignedCube& AA)
		{
			Min = AA.Min;
			Max = AA.Max;
			return *this;
		}
	};

	template<class TAxisAlignedCube>
	concept TIsAxisAlignedCube = TIsMatrixTyped<TAxisAlignedCube, T> && (TAxisAlignedCube::Row() >= 2);

	template<>
	struct AxisAlignedCube<void, 0>
	{
		template<TIsAxisAlignedCube IMatrix>
		static String ToString(const IMatrix& M, String FormatArgs = TEXT(""))
		{
			return String::Format(L"Min: {}, Max: {}", Vector<>::ToString(M[0], FormatArgs), Vector<>::ToString(M[1], FormatArgs));
		}

		template<TIsAxisAlignedCube IMatrix>
		static constexpr auto Center(const IMatrix& M)
		{
			return (M[0] + M[1]) * 0.5f;
		}

		template<TIsAxisAlignedCube IMatrix>
		static constexpr auto Extent(const IMatrix& M)
		{
			return (M[1] - M[0]) * 0.5f;
		}
	};

	using AxisAlignedCubeF = AxisAlignedCube<float, 2>;
	using AxisAlignedCubeD = AxisAlignedCube<double, 2>;
	using BoundingBoxF = AxisAlignedCube<float, 3>;
	using BoundingBoxD = AxisAlignedCube<double, 3>;
}