// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"

namespace Ayla
{
	template<class T, size_t N = 0>
	struct Line
	{
		Vector<T, N> Start;
		Vector<T, N> End;

	public:
		using Type = T;
		using VectorType = Vector<T, N>;

		constexpr Line(const VectorType& S = VectorType{}) : Start(S), End(S)
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

		constexpr Line operator -() const
		{
			return Line(-Start, -End);
		}

		constexpr const VectorType& operator [](size_t I) const
		{
			return I == 0 ? Start : End;
		}

		constexpr VectorType& operator [](size_t I)
		{
			return I == 0 ? Start : End;
		}

		template<TIsMatrix<T, 2, N> IMatrix>
		constexpr Line& operator =(const IMatrix& M)
		{
			Start = M[0];
			End = M[1];
			return *this;
		}

		static constexpr Line Identity()
		{
			Line R(0.0);
			R[0][0] = 1.0;
			if constexpr (N > 1)
			{
				R[1][1] = 1.0;
			}
			return R;
		}

	public:
		constexpr Line(const VectorType& Start, const VectorType& End) : Start(Start), End(End)
		{
		}

		constexpr Line(const Line& L)
			: Start(L.Start)
			, End(L.End)
		{
		}

		template<TIsMatrixTyped<T> IMatrix>
		constexpr Line(const IMatrix& M) requires (IMatrix::Row() >= 2)
			: Start(M[0]), End(M[1])
		{
		}

		constexpr Line& operator =(const Line& L)
		{
			Start = L.Start;
			End = L.End;
			return *this;
		}
	};

	template<class TLine>
	concept TIsLine = TIsMatrixTyped<TLine, typename TLine::Type> && (TLine::Row() >= 2);

	template<>
	struct Line<void, 0>
	{
		template<TIsLine IMatrix>
		static String ToString(const IMatrix& M, String FormatArgs = TEXT(""))
		{
			return String::Format(L"Start: {}, End: {}", Vector<>::ToString(M[0], FormatArgs), Vector<>::ToString(M[1], FormatArgs));
		}

		template<TIsLine IMatrix, class T>
		static constexpr auto GetPoint(const IMatrix& L, T D)
		{
			return L[0] + GetVector(L) * D;
		}

		template<TIsLine IMatrix>
		static constexpr auto GetVector(const IMatrix& L)
		{
			return L[1] - L[0];
		}
	};

	using Line2F = Line<float, 2>;
	using Line2D = Line<double, 2>;
	using Line3F = Line<float, 3>;
	using Line3D = Line<double, 3>;
}