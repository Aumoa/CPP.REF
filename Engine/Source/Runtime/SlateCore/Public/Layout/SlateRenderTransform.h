// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/TransformInterface/Translate2D.h"
#include "Numerics/TransformInterface/Scale2D.h"
#include "Numerics/TransformInterface/Shear2D.h"
#include "Numerics/TransformInterface/Complex.h"
#include "Numerics/MatrixInterface/Matrix2x2.h"

struct NSlateRenderTransform
{
	constexpr NSlateRenderTransform()
	{
	}

	constexpr NSlateRenderTransform(const NSlateRenderTransform& Rhs) noexcept
		: M(Rhs.M)
		, Translation(Rhs.Translation)
	{
	}

	constexpr NSlateRenderTransform(const Translate2D& Translation)
		: M(Matrix2x2::Identity())
		, Translation(Translation)
	{
	}

	constexpr NSlateRenderTransform(float UniformScale, const Translate2D& Translation = Translate2D::Identity())
		: NSlateRenderTransform(Scale2D(UniformScale), Translation)
	{
	}

	constexpr NSlateRenderTransform(const Scale2D& Scale, const Translate2D& Translation = Translate2D::Identity())
		: M(Matrix2x2::Scale(Scale))
		, Translation(Translation)
	{
	}

	constexpr NSlateRenderTransform(const Shear2D& Shear, const Translate2D& Translation = Translate2D::Identity())
		: M(Matrix2x2::Shear(Shear))
		, Translation(Translation)
	{
	}

	constexpr NSlateRenderTransform(const Complex& Rotation, const Translate2D& Translation = Translate2D::Identity())
		: M(Matrix3x2::Rotation<Matrix2x2>(Rotation))
		, Translation(Translation)
	{
	}

	constexpr NSlateRenderTransform(const Matrix2x2& Transform, const Translate2D& Translation)
		: M(Transform)
		, Translation(Translation)
	{
	}

	constexpr NSlateRenderTransform(const Matrix3x2& Transform)
		: M(Transform)
		, Translation(Transform[2][0], Transform[2][1])
	{
	}

	constexpr auto operator <=>(const NSlateRenderTransform&) const noexcept = default;
	constexpr bool operator ==(const NSlateRenderTransform&) const noexcept = default;
	constexpr NSlateRenderTransform& operator =(const NSlateRenderTransform&) noexcept = default;

public:
	static constexpr NSlateRenderTransform Inverse(const NSlateRenderTransform& T)
	{
		Matrix2x2 IM = Matrix<>::Inverse(T.M);
		Translate2D IT = Matrix<>::TransformVector(IM, -T.Translation);
		return NSlateRenderTransform(IM, IT);
	}

	constexpr NSlateRenderTransform Inverse() const
	{
		return Inverse(*this);
	}

	static constexpr NSlateRenderTransform Identity()
	{
		return
		{
			Matrix2x2::Identity(),
			Translate2D::Identity()
		};
	}

	static constexpr NSlateRenderTransform Concatenate(const NSlateRenderTransform& TL, const NSlateRenderTransform& TR)
	{
		return NSlateRenderTransform
		{
			Matrix<>::Multiply(TL.M, TR.M),
			Translate2D::Concatenate(Matrix<>::TransformVector(TR.M, TL.Translation), TR.Translation)
		};
	}

	constexpr NSlateRenderTransform Concatenate(const NSlateRenderTransform& T) const
	{
		return Concatenate(*this, T);
	}

	template<TIsVector<float, 2> IPoint>
	static constexpr IPoint TransformPoint(const NSlateRenderTransform& T, const IPoint& Point)
	{
		return T.Translation.TransformPoint(Matrix<>::TransformVector(T.M, Point));
	}

	template<TIsVector<float, 2> IPoint>
	constexpr IPoint TransformPoint(const IPoint& Point) const
	{
		return TransformPoint(*this, Point);
	}

	template<TIsVector<float, 2> INormal>
	static constexpr INormal TransformVector(const NSlateRenderTransform& T, const INormal& Normal)
	{
		return T.M.TransformVector(Normal);
	}

	template<TIsVector<float, 2> INormal>
	constexpr INormal TransformVector(const INormal& Vector) const
	{
		return TransformVector(*this, Vector);
	}

public:
	String ToString() const
	{
		return String::Format(TEXT("M: {}, Translation: {}"), M.ToString(), Translation.ToString());
	}

	static constexpr bool IsIdentity(const NSlateRenderTransform& T, float Epsilon = 0)
	{
		return Matrix<>::IsIdentity(T.M, Epsilon) && T.Translation.NearlyEquals(T.Translation.Identity(), Epsilon);
	}

	constexpr bool IsIdentity(float Epsilon = 0) const
	{
		return IsIdentity(*this, Epsilon);
	}

	template<class ISlateLayoutTransform>
	static constexpr NSlateRenderTransform Concatenate(const NSlateRenderTransform& TL, const ISlateLayoutTransform& TR) requires requires
	{
		{ std::declval<ISlateLayoutTransform>().Scale };
		{ std::declval<ISlateLayoutTransform>().TransformPoint(std::declval<Translate2D>()) };
	}
	{
		return NSlateRenderTransform(
			Matrix<>::Multiply(TL.M, Matrix2x2::Scale(TR.Scale)),
			TR.TransformPoint(TL.Translation)
		);
	}

	template<class ISlateLayoutTransform>
	constexpr NSlateRenderTransform Concatenate(const ISlateLayoutTransform& T) const requires requires
	{
		{ std::declval<ISlateLayoutTransform>().Scale };
		{ std::declval<ISlateLayoutTransform>().TransformPoint(std::declval<Translate2D>()) };
	}
	{
		return Concatenate(*this, T);
	}

	static constexpr NSlateRenderTransform Concatenate(const Translate2D& Translation, const NSlateRenderTransform& T)
	{
		return NSlateRenderTransform(
			T.M,
			T.M.TransformPoint(Translation).Concatenate(T.Translation)
		);
	}

	Matrix2x2 M = Matrix2x2::Identity();
	Translate2D Translation = Translate2D::Identity();
};