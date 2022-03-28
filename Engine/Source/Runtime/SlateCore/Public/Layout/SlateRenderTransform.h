// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CoreNumerics.h"

struct SlateRenderTransform
{
	constexpr SlateRenderTransform()
	{
	}

	constexpr SlateRenderTransform(const Translate2D& Translation)
		: M(Matrix2x2::Identity())
		, Translation(Translation)
	{
	}

	constexpr SlateRenderTransform(float UniformScale, const Translate2D& Translation = Translate2D::Identity())
		: SlateRenderTransform(Scale2D(UniformScale), Translation)
	{
	}

	constexpr SlateRenderTransform(const Scale2D& Scale, const Translate2D& Translation = Translate2D::Identity())
		: M(Matrix2x2::Scale(Scale))
		, Translation(Translation)
	{
	}

	constexpr SlateRenderTransform(const Shear2D& Shear, const Translate2D& Translation = Translate2D::Identity())
		: M(Matrix2x2::Shear(Shear))
		, Translation(Translation)
	{
	}

	constexpr SlateRenderTransform(const Complex& Rotation, const Translate2D& Translation = Translate2D::Identity())
		: M(Matrix3x2::Rotation<Matrix2x2>(Rotation))
		, Translation(Translation)
	{
	}

	constexpr SlateRenderTransform(const Matrix2x2& Transform, const Translate2D& Translation)
		: M(Transform)
		, Translation(Translation)
	{
	}

	constexpr SlateRenderTransform(const Matrix3x2& Transform)
		: M(Transform)
		, Translation(Transform[2][0], Transform[2][1])
	{
	}

	constexpr auto operator <=>(const SlateRenderTransform&) const = default;

public:
	static constexpr SlateRenderTransform Inverse(const SlateRenderTransform& T)
	{
		Matrix2x2 IM = Matrix<>::Inverse(T.M);
		Translate2D IT = Matrix<>::TransformVector(IM, -T.Translation);
		return SlateRenderTransform(IM, IT);
	}

	constexpr SlateRenderTransform Inverse() const
	{
		return Inverse(*this);
	}

	static constexpr SlateRenderTransform Identity()
	{
		return
		{
			Matrix2x2::Identity(),
			Translate2D::Identity()
		};
	}

	static constexpr SlateRenderTransform Concatenate(const SlateRenderTransform& TL, const SlateRenderTransform& TR)
	{
		return SlateRenderTransform
		{
			Matrix<>::Multiply(TL.M, TR.M),
			Translate2D::Concatenate(Matrix<>::TransformVector(TR.M, TL.Translation), TR.Translation)
		};
	}

	constexpr SlateRenderTransform Concatenate(const SlateRenderTransform& T) const
	{
		return Concatenate(*this, T);
	}

	template<TIsVector<float, 2> IPoint>
	static constexpr IPoint TransformPoint(const SlateRenderTransform& T, const IPoint& Point)
	{
		return T.Translation.TransformPoint(Matrix<>::TransformVector(T.M, Point));
	}

	template<TIsVector<float, 2> IPoint>
	constexpr IPoint TransformPoint(const IPoint& Point) const
	{
		return TransformPoint(*this, Point);
	}

	template<TIsVector<float, 2> INormal>
	static constexpr INormal TransformVector(const SlateRenderTransform& T, const INormal& Normal)
	{
		return T.M.TransformVector(Normal);
	}

	template<TIsVector<float, 2> INormal>
	constexpr INormal TransformVector(const INormal& Vector) const
	{
		return TransformVector(*this, Vector);
	}

public:
	std::wstring ToString(std::wstring_view InFormatArgs = L"") const
	{
		return String::Format(L"M: {}, Translation: {}", M.ToString(InFormatArgs), Translation.ToString(InFormatArgs));
	}

	static constexpr bool IsIdentity(const SlateRenderTransform& T, float Epsilon = 0)
	{
		return Matrix<>::IsIdentity(T.M, Epsilon) && T.Translation.NearlyEquals(T.Translation.Identity(), Epsilon);
	}

	constexpr bool IsIdentity(float Epsilon = 0) const
	{
		return IsIdentity(*this, Epsilon);
	}

	template<class ISlateLayoutTransform>
	static constexpr SlateRenderTransform Concatenate(const SlateRenderTransform& TL, const ISlateLayoutTransform& TR) requires requires
	{
		{ std::declval<ISlateLayoutTransform>().Scale };
		{ std::declval<ISlateLayoutTransform>().TransformPoint(std::declval<Translate2D>()) };
	}
	{
		return SlateRenderTransform(
			Matrix<>::Multiply(TL.M, Matrix2x2::Scale(TR.Scale)),
			TR.TransformPoint(TL.Translation)
		);
	}

	template<class ISlateLayoutTransform>
	constexpr SlateRenderTransform Concatenate(const ISlateLayoutTransform& T) const requires requires
	{
		{ std::declval<ISlateLayoutTransform>().Scale };
		{ std::declval<ISlateLayoutTransform>().TransformPoint(std::declval<Translate2D>()) };
	}
	{
		return Concatenate(*this, T);
	}

	static constexpr SlateRenderTransform Concatenate(const Translate2D& Translation, const SlateRenderTransform& T)
	{
		return SlateRenderTransform(
			T.M,
			T.M.TransformPoint(Translation).Concatenate(T.Translation)
		);
	}

	Matrix2x2 M = Matrix2x2::Identity();
	Translate2D Translation = Translate2D::Identity();
};