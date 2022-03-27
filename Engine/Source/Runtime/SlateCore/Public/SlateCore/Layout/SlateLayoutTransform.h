// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Numerics/TransformInterface/Scale2D.h"
#include "Numerics/TransformInterface/Translate2D.h"
#include "Numerics/MatrixInterface/Matrix3x2.h"

struct SlateLayoutTransform
{
	Scale2D Scale = Scale2D::Identity();
	Translate2D Translation = Translate2D::Identity();

	constexpr SlateLayoutTransform()
	{
	}

	constexpr SlateLayoutTransform(const Scale2D& Scale, const Translate2D& Translation)
		: Scale(Scale)
		, Translation(Translation)
	{
	}

	constexpr SlateLayoutTransform(const Translate2D& Translation)
		: Scale(1.0f)
		, Translation(Translation)
	{
	}

	std::wstring ToString(std::wstring_view InFormatArgs = L"") const
	{
		return String::Format(L"Translation: {}, Scale: {}", Translation.ToString(InFormatArgs), String::Format(String::GetPlaceholder(InFormatArgs), Scale.ToString(InFormatArgs)));
	}

	constexpr auto operator <=>(const SlateLayoutTransform& Rhs) const = default;

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
	static constexpr SlateLayoutTransform Inverse(const SlateLayoutTransform& T)
	{
		Scale2D InvScale = T.Scale.Inverse();
		return SlateLayoutTransform(InvScale, InvScale.TransformPoint(T.Translation.Inverse()));
	}

	constexpr SlateLayoutTransform Inverse() const
	{
		return Inverse(*this);
	}

	static constexpr SlateLayoutTransform Identity()
	{
		return SlateLayoutTransform(Scale2D::Identity(), Translate2D::Identity());
	}

	static constexpr SlateLayoutTransform Concatenate(const SlateLayoutTransform& TL, const SlateLayoutTransform& TR)
	{
		return SlateLayoutTransform(TL.Scale.Concatenate(TR.Scale), TR.TransformPoint(TL.Translation));
	}

	constexpr SlateLayoutTransform Concatenate(const SlateLayoutTransform& T) const
	{
		return Concatenate(*this, T);
	}

	template<TIsVector<float, 2> IVector>
	static constexpr IVector TransformPoint(const SlateLayoutTransform& T, const IVector& Point)
	{
		return T.Translation.TransformPoint(T.Scale.TransformPoint(Point));
	}

	template<TIsVector<float, 2> IVector>
	constexpr IVector TransformPoint(const IVector& Point) const
	{
		return TransformPoint(*this, Point);
	}

	template<TIsVector<float, 2> IVector>
	static constexpr IVector TransformVector(const SlateLayoutTransform& T, const IVector& Vector)
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
	static constexpr ISlateRenderTransform Concatenate(const SlateLayoutTransform& TL, const ISlateRenderTransform& TR) requires requires
	{
		{ ISlateRenderTransform(std::declval<SlateLayoutTransform>().ToMatrix()) };
		{ std::declval<ISlateRenderTransform>().Concatenate(std::declval<ISlateRenderTransform>()) };
	}
	{
		return ISlateRenderTransform(TL.ToMatrix()).Concatenate(TR);
	}

	template<class ISlateRenderTransform>
	constexpr SlateRenderTransform Concatenate(const ISlateRenderTransform& T) const requires requires
	{
		Concatenate(std::declval<SlateLayoutTransform>(), std::declval<ISlateRenderTransform>());
	}
	{
		return Concatenate(*this, T);
	}
};