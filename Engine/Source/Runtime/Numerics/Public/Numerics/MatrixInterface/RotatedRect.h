// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Numerics/VectorInterface/Rect.h"
#include "Numerics/TransformConcepts.h"

namespace Ayla
{
	template<class T>
	struct RotatedRect
	{
		Vector2<T> TopLeft;
		Vector2<T> ExtentX;
		Vector2<T> ExtentY;

		constexpr RotatedRect(const Vector2<T>& TopLeft, const Vector2<T>& ExtentX, const Vector2<T>& ExtentY)
			: TopLeft(TopLeft)
			, ExtentX(ExtentX)
			, ExtentY(ExtentY)
		{
		}

		constexpr RotatedRect(const Rect& R)
			: TopLeft(Rect::LeftTop(R))
			, ExtentX(R.Right - R.Left, 0)
			, ExtentY(0, R.Bottom - R.Top)
		{
		}

		constexpr RotatedRect(const RotatedRect& R)
			: TopLeft(R.TopLeft)
			, ExtentX(R.ExtentX)
			, ExtentY(R.ExtentY)
		{
		}

		template<TIsMatrix<T, 3, 2> IMatrix>
		constexpr RotatedRect(const IMatrix& M)
			: TopLeft(M[0])
			, ExtentX(M[1])
			, ExtentY(M[2])
		{
		}

		constexpr RotatedRect& operator =(const RotatedRect& R)
		{
			TopLeft = R.TopLeft;
			ExtentX = R.ExtentX;
			ExtentY = R.ExtentY;
			return *this;
		}

	public:
		using Type = T;
		using VectorType = Vector2<T>;

		constexpr RotatedRect(T S) : RotatedRect(Vector2<T>(S))
		{
		}

		constexpr RotatedRect(const Vector2<T>& S = 0)
			: TopLeft(S)
			, ExtentX(S)
			, ExtentY(S)
		{
		}

		static constexpr size_t Row()
		{
			return 3;
		}

		static constexpr size_t Column()
		{
			return 2;
		}

		constexpr RotatedRect operator -() const
		{
			return RotatedRect(-TopLeft, -ExtentX, -ExtentY);
		}

		constexpr const VectorType& operator [](size_t N) const
		{
			switch (N)
			{
			case 0:
				return TopLeft;
			case 1:
				return ExtentX;
			case 2:
			default:
				return ExtentY;
			}
		}

		constexpr VectorType& operator [](size_t N)
		{
			switch (N)
			{
			case 0:
				return TopLeft;
			case 1:
				return ExtentX;
			case 2:
			default:
				return ExtentY;
			}
		}

		template<TIsMatrix<T, 3, 2> IMatrix>
		constexpr RotatedRect& operator =(const IMatrix& M)
		{
			TopLeft = M[0];
			ExtentX = M[1];
			ExtentY = M[2];
			return *this;
		}

		static constexpr RotatedRect Identity()
		{
			return { Vector2<T>::Zero() };
		}

	public:
		template<TIsMatrix<T, 3, 2> IMatrix>
		static constexpr Rect ToBoundingRect(const IMatrix& M)
		{
			const Vector2<T> Points[] =
			{
				M[0],
				M[0] + M[1],
				M[0] + M[2],
				M[0] + M[1] + M[2]
			};

			return Rect
			{
				Math::Min(Points[0].X, Points[1].X, Points[2].X, Points[3].X),
				Math::Min(Points[0].Y, Points[1].Y, Points[2].Y, Points[3].Y),
				Math::Max(Points[0].X, Points[1].X, Points[2].X, Points[3].X),
				Math::Max(Points[0].Y, Points[1].Y, Points[2].Y, Points[3].Y)
			};
		}

		constexpr Rect ToBoundingRect() const
		{
			return ToBoundingRect(*this);
		}

		template<TIsMatrix<T, 3, 2> IMatrix, TIsVector<T, 2> IVector>
		static constexpr bool IsUnderLocation(const IMatrix& M, const IVector& V)
		{
			const IVector Offset = V - M[0];
			const auto Det = Vector<>::Cross(M[1], M[2]);
			const auto InvDet = (decltype(Det))1.0 / Det;

			const auto S = -Vector<>::Cross(Offset, M[1]) * InvDet;
			if (Math::IsWithinInclusive(S, 0.0, 1.0))
			{
				const auto T = Vector<>::Cross(Offset, M[2]) * InvDet;
				return Math::IsWithinInclusive(T, 0.0, 1.0);
			}
			return false;
		}

		template<TIsVector<T, 2> IVector>
		constexpr bool IsUnderLocation(const IVector& V) const
		{
			return IsUnderLocation(*this, V);
		}

		template<TIsVector<T, 4> IRect, TIsTransform<Rect> ITransform>
		static constexpr RotatedRect MakeRotatedRect(const ITransform& T, const IRect& R)
		{
			const RotatedRect RRect = Rect::TransformRect<Rect>(T, R);

			const Vector2<T> TopRight = RRect.TopLeft + RRect.ExtentX;
			const Vector2<T> BottomLeft = RRect.TopLeft + RRect.ExtentY;

			return RotatedRect
			{
				RRect.TopLeft,
				TopRight - RRect.TopLeft,
				BottomLeft - RRect.TopLeft
			};
		}

		template<TIsMatrix<T, 3, 2> IRotatedRectResult = RotatedRect
			, TIsMatrix<T, 3, 2> IRotatedRect
			, TIsTransform<Vector2<T>> ITransform>
		static constexpr IRotatedRectResult TransformRect(const ITransform& T, const IRotatedRect& R)
		{
			IRotatedRectResult Result;
			Result[0] = T.TransformPoint(R[0]);
			Result[1] = T.TransformVector(R[1]);
			Result[2] = T.TransformVector(R[2]);
			return Result;
		}
	};

	using RotatedRectF = RotatedRect<float>;
	using RotatedRectD = RotatedRect<double>;
}