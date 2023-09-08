// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/TransformInterface/Scale2D.h"
#include "Numerics/TransformInterface/Translate2D.h"

struct NSlateLayoutTransform
{
	Scale2D Scale = Scale2D::Identity();
	Translate2D Translation = Translate2D::Identity();

	constexpr NSlateLayoutTransform()
	{
	}

	constexpr NSlateLayoutTransform(const Scale2D& Scale, const Translate2D& Translation)
		: Scale(Scale)
		, Translation(Translation)
	{
	}

	constexpr NSlateLayoutTransform(const Translate2D& Translation)
		: Scale(1.0f)
		, Translation(Translation)
	{
	}

	String ToString() const
	{
		return String::Format(TEXT("Translation: {}, Scale: {}"), Translation.ToString(), Scale.ToString());
	}

	constexpr auto operator <=>(const NSlateLayoutTransform& Rhs) const = default;

	constexpr Matrix3x2 ToMatrix() const
	{
		return
		{
			Scale.X, 0.0f,
			0.0f, Scale.Y,
			Translation.X, Translation.Y
		};
	}

public:
	static constexpr NSlateLayoutTransform Inverse(const NSlateLayoutTransform& T)
	{
		Scale2D InvScale = T.Scale.Inverse();
		return NSlateLayoutTransform(InvScale, InvScale.TransformPoint(T.Translation.Inverse()));
	}

	constexpr NSlateLayoutTransform Inverse() const
	{
		return Inverse(*this);
	}

	static constexpr NSlateLayoutTransform Identity()
	{
		return NSlateLayoutTransform(Scale2D::Identity(), Translate2D::Identity());
	}

	static constexpr NSlateLayoutTransform Concatenate(const NSlateLayoutTransform& TL, const NSlateLayoutTransform& TR)
	{
		return NSlateLayoutTransform(TL.Scale.Concatenate(TR.Scale), TR.TransformPoint(TL.Translation));
	}

	constexpr NSlateLayoutTransform Concatenate(const NSlateLayoutTransform& T) const
	{
		return Concatenate(*this, T);
	}

	template<TIsVector<float, 2> IVector>
	static constexpr IVector TransformPoint(const NSlateLayoutTransform& T, const IVector& Point)
	{
		return T.Translation.TransformPoint(T.Scale.TransformPoint(Point));
	}

	template<TIsVector<float, 2> IVector>
	constexpr IVector TransformPoint(const IVector& Point) const
	{
		return TransformPoint(*this, Point);
	}

	template<TIsVector<float, 2> IVector>
	static constexpr IVector TransformVector(const NSlateLayoutTransform& T, const IVector& Vector)
	{
		return T.Translation.TransformVector(T.Scale.TransformVector(Vector));
	}

	template<TIsVector<float, 2> IVector>
	constexpr IVector TransformVector(const IVector& Vector) const
	{
		return TransformVector(*this, Vector);
	}

public:
	template<class ISlateRenderTransform>
	static constexpr ISlateRenderTransform Concatenate(const NSlateLayoutTransform& TL, const ISlateRenderTransform& TR) requires requires
	{
		{ ISlateRenderTransform(std::declval<NSlateLayoutTransform>().ToMatrix()) };
		{ std::declval<ISlateRenderTransform>().Concatenate(std::declval<ISlateRenderTransform>()) };
	}
	{
		return ISlateRenderTransform(TL.ToMatrix()).Concatenate(TR);
	}

	template<class ISlateRenderTransform>
	constexpr NSlateRenderTransform Concatenate(const ISlateRenderTransform& T) const requires requires
	{
		Concatenate(std::declval<NSlateLayoutTransform>(), std::declval<ISlateRenderTransform>());
	}
	{
		return Concatenate(*this, T);
	}
};