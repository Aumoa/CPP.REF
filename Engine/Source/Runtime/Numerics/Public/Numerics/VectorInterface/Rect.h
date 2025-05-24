// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/TransformConcepts.h"
#include "Numerics/VectorInterface/Vector.h"

namespace Ayla
{
	template<class T = void>
	struct Rect
	{
		T Left;
		T Top;
		T Right;
		T Bottom;

		constexpr Rect(const T& Left, const T& Top, const T& Right, const T& Bottom)
			: Left(Left)
			, Top(Top)
			, Right(Right)
			, Bottom(Bottom)
		{
		}

		constexpr Rect(const Rect& Rhs)
			: Left(Rhs.Left)
			, Top(Rhs.Top)
			, Right(Rhs.Right)
			, Bottom(Rhs.Bottom)
		{
		}

		template<TIsVector<T, 2> IVectorLT, TIsVector<T, 2> IVectorRB>
		constexpr Rect(const IVectorLT& LT, const IVectorRB& RB)
			: Left(LT[0]), Top(LT[1])
			, Right(RB[0]), Bottom(RB[1])
		{
		}

		template<TIsVector<T, 4> IVector>
		constexpr Rect(const IVector& V) : Rect(V[0], V[1], V[2], V[3])
		{
		}

	public:
		using Type = T;

		constexpr Rect(const T& S = T{}) : Left(S), Top(S), Right(S), Bottom(S)
		{
		}

		static constexpr size_t Size()
		{
			return 4;
		}

	public:
		template<TIsVector<T, 2> IPoint>
		constexpr bool Contains(const IPoint& P) const;

		template<TIsVector<Type, 2> IExtent>
		constexpr Rect Extend(const IExtent& E) const
		{
			return Extend(*this, E);
		}

		template<TIsVector<float, 4> IRectResult = Rect, TIsTransform<Vector2<T>> ITransform2D, TIsVector<float, 4> IRect>
		static constexpr IRectResult TransformRect(const ITransform2D& Transform, const IRect& InRect)
		{
			const auto TL = Transform.TransformPoint(Rect::LeftTop(InRect));
			const auto RB = Transform.TransformPoint(Rect::RightBottom(InRect));

			IRectResult R;
			R[0] = Math::Min(TL.X, RB.X);
			R[1] = Math::Min(TL.Y, RB.Y);
			R[2] = Math::Max(TL.X, RB.X);
			R[3] = Math::Max(TL.Y, RB.Y);
			return R;
		}

	public:
		constexpr Rect operator -() const
		{
			return Rect(-Left, -Top, -Right, -Bottom);
		}

		constexpr const T& operator [](size_t N) const
		{
			switch (N)
			{
			case 0:
				return Left;
			case 1:
				return Top;
			case 2:
				return Right;
			case 3:
			default:
				return Bottom;
			}
		}

		constexpr T& operator [](size_t N)
		{
			switch (N)
			{
			case 0:
				return Left;
			case 1:
				return Top;
			case 2:
				return Right;
			case 3:
			default:
				return Bottom;
			}
		}

		template<TIsVector<T, 4> IRect>
		constexpr Rect& operator =(const IRect& R)
		{
			Left = R[0];
			Top = R[1];
			Right = R[2];
			Bottom = R[3];
			return *this;
		}

	public:
		String ToString(String formatArgs) const;
	};

	template<>
	struct Rect<void>
	{
		template<TIsVectorSized<4> IRectL, TIsVectorSized<4> IRectR>
		static constexpr auto NearlyEquals(const IRectL& RL, const IRectR& RR, const typename IRectL::Type& Epsilon) requires TIsCompatibleVector<IRectL, IRectR>
		{
			return Vector<>::NearlyEquals(RL, RR, Epsilon);
		}

		template<TIsVectorSized<4> IRect>
		static constexpr auto Width(const IRect& R)
		{
			return Math::Abs(R[2] - R[0]);
		}

		template<TIsVectorSized<4> IRect>
		static constexpr auto Height(const IRect& R)
		{
			return Math::Abs(R[3] - R[1]);
		}

		template<TIsVectorSized<4> IRect>
		static constexpr auto LeftTop(const IRect& R)
		{
			return Vector<>::Swizzling<0, 1>(R);
		}

		template<TIsVectorSized<4> IRect>
		static constexpr auto RightBottom(const IRect& R)
		{
			return Vector<>::Swizzling<2, 3>(R);
		}

		template<TIsVectorSized<4> IRect>
		static constexpr auto Center(const IRect& R)
		{
			// IntermediateVector
			using TVector = Vector<typename IRect::Type, 2>;
			using TIntVector = Vector<double, 2>;

			return Vector<>::Cast<TVector>
			(
				(
					Vector<>::Cast<TIntVector>(RightBottom(R)) +
					Vector<>::Cast<TIntVector>(LeftTop(R))
				) * 0.5
			);
		}

		template<TIsVectorSized<4> IRect>
		static String ToString(const IRect& R, String FormatArgs = TEXT(""))
		{
			auto LT = LeftTop(R);
			auto RB = RightBottom(R);
			auto W = Width(R);
			auto H = Height(R);

			return String::Format(L"{{LT: {}, RB: {}, [{} * {}]}}", Vector<>::ToString(LT, FormatArgs), Vector<>::ToString(RB, FormatArgs), W, H);
		}

		template<TIsVectorSized<4> IRect, TIsVectorSized<2> IPoint>
		static constexpr bool Contains(const IRect& R, const IPoint& P) requires std::same_as<typename IRect::Type, typename IPoint::Type>
		{
			return R[0] <= P[0] && P[0] <= R[2]
				&& R[1] <= P[1] && P[1] <= R[3];
		}

		template<TIsVectorSized<4> IRectL, TIsVectorSized<4> IRectR>
		static constexpr bool IsIntersect(const IRectL& RL, const IRectR& RR) requires std::same_as<typename IRectL::Type, typename IRectR::Type>
		{
			const auto L = Math::Max(RL[0], RR[0]);
			const auto T = Math::Max(RL[1], RR[1]);
			const auto R = Math::Min(RL[2], RR[2]);
			const auto B = Math::Min(RL[3], RR[3]);

			if (R >= L && B >= T)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		template<TIsVectorSized<4> IRectL, TIsVectorSized<4> IRectR>
		static constexpr std::optional<IRectL> Intersect(const IRectL& RL, const IRectR& RR) requires std::same_as<typename IRectL::Type, typename IRectR::Type>
		{
			const auto L = Math::Max(RL[0], RR[0]);
			const auto T = Math::Max(RL[1], RR[1]);
			const auto R = Math::Min(RL[2], RR[2]);
			const auto B = Math::Min(RL[3], RR[3]);

			if (R >= L && B >= T)
			{
				IRectL Result;
				Result[0] = L;
				Result[1] = T;
				Result[2] = R;
				Result[3] = B;
				return Result;
			}
			else
			{
				return std::nullopt;
			}
		}

		template<TIsVectorSized<4> IRectL, TIsVectorSized<2> IExtent>
		static constexpr IRectL Extend(const IRectL& R, const IExtent& E) requires
			std::same_as<typename IRectL::Type, typename IExtent::Type> &&
			(IExtent::Size() < 4)
		{
			using T = typename IRectL::Type;
			auto E0 = (T)((double)E[0] * 0.5);
			auto E1 = (T)((double)E[1] * 0.5);

			IRectL V = R;
			V[0] -= E0;
			V[1] -= E1;
			V[2] += E0;
			V[3] += E1;
			return V;
		}

		template<TIsVectorSized<4> IRectL, TIsVectorSized<4> IExtent>
		static constexpr IRectL Extend(const IRectL& R, const IExtent& E) requires
			std::same_as<typename IRectL::Type, typename IExtent::Type>
		{
			IRectL V = R;
			V[0] -= E[0];
			V[1] -= E[1];
			V[2] += E[2];
			V[3] += E[3];
			return V;
		}

		template<TIsVectorSized<4> IRect>
		static constexpr IRect Margin(const IRect& v, typename IRect::Type left, typename IRect::Type top, typename IRect::Type right, typename IRect::Type bottom)
		{
			auto r = v;
			r[0] += left;
			r[1] += top;
			r[2] -= right;
			r[3] -= bottom;
			return r;
		}

		template<TIsVectorSized<4> IRect>
		static constexpr IRect MarginLeft(const IRect& v, typename IRect::Type left)
		{
			auto r = v;
			r[0] += left;
			return r;
		}

		template<TIsVectorSized<4> IRect>
		static constexpr IRect MarginTop(const IRect& v, typename IRect::Type top)
		{
			auto r = v;
			r[1] += top;
			return r;
		}

		template<TIsVectorSized<4> IRect>
		static constexpr IRect MarginRight(const IRect& v, typename IRect::Type right)
		{
			auto r = v;
			r[2] -= right;
			return r;
		}

		template<TIsVectorSized<4> IRect>
		static constexpr IRect MarginBottom(const IRect& v, typename IRect::Type bottom)
		{
			auto r = v;
			r[3] -= bottom;
			return r;
		}

		template<TIsVectorSized<4> IRect>
		static constexpr IRect FillLeft(const IRect& v, typename IRect::Type left)
		{
			auto r = v;
			r[2] = r[0] + left;
			return r;
		}

		template<TIsVectorSized<4> IRect>
		static constexpr IRect FillTop(const IRect& v, typename IRect::Type top)
		{
			auto r = v;
			r[3] = r[1] + top;
			return r;
		}

		template<TIsVectorSized<4> IRect>
		static constexpr IRect FillRight(const IRect& v, typename IRect::Type right)
		{
			auto r = v;
			r[0] = r[2] - right;
			return r;
		}

		template<TIsVectorSized<4> IRect>
		static constexpr IRect FillBottom(const IRect& v, typename IRect::Type bottom)
		{
			auto r = v;
			r[1] = r[3] - bottom;
			return r;
		}
	};

	template<class T>
	template<TIsVector<T, 2> IPoint>
	constexpr bool Rect<T>::Contains(const IPoint& P) const
	{
		return Rect<void>::Contains(*this, P);
	}

	using RectF = Rect<float>;
	using RectD = Rect<double>;
	using RectN = Rect<int32>;
}