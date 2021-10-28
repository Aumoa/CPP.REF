// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "TransformCalculus2D.h"
#include "Scale2D.h"
#include "SlateRenderTransform.h"

struct SlateLayoutTransform
{
	constexpr SlateLayoutTransform()
	{
	}

	constexpr SlateLayoutTransform(float Scale, const Vector2& Translation)
		: Scale(Scale)
		, Translation(Translation)
	{
	}

	constexpr SlateLayoutTransform(const Vector2& Translation)
		: Scale(1.0f)
		, Translation(Translation)
	{
	}

	std::wstring ToString(std::wstring_view InFormatArgs = L"") const
	{
		return std::format(L"Translation: {}, Scale: {}", Translation.ToString(InFormatArgs), std::format(StringUtils::GetPlaceholder(InFormatArgs), Scale));
	}

	template<class T>
	constexpr T TransformPoint(const T& Point) const
	{
		return TransformCalculus2D::TransformPoint(Translation, Scale2D(Scale).TransformPoint(Point));
	}

	template<class T>
	constexpr T TransformVector(const T& Point) const
	{
		return TransformCalculus2D::TransformVector(Translation, Scale2D(Scale).TransformVector(Point));
	}

	constexpr SlateLayoutTransform Concatenate(const SlateLayoutTransform& Rhs) const
	{
		return SlateLayoutTransform(TransformCalculus2D::Concatenate(Scale, Rhs.Scale), Rhs.TransformPoint(Translation));
	}

	constexpr SlateLayoutTransform Inverse() const
	{
		float InvScale = 1.0f / Scale;
		return SlateLayoutTransform(InvScale, -Translation * InvScale);
	}

	constexpr bool operator ==(const SlateLayoutTransform& Rhs) const
	{
		return Scale == Rhs.Scale && Translation == Rhs.Translation;
	}

	constexpr bool operator !=(const SlateLayoutTransform& Rhs) const
	{
		return Scale != Rhs.Scale || Translation != Rhs.Translation;
	}

	constexpr float GetScale() const
	{
		return Scale;
	}

	constexpr const Vector2& GetTranslation() const
	{
		return Translation;
	}

	constexpr Matrix3x2 ToMatrix() const
	{
		return
		{
			Scale, 0.0f,
			0.0f, Scale,
			Translation.X, Translation.Y
		};
	}
	
	static constexpr SlateLayoutTransform Identity()
	{
		return SlateLayoutTransform(1.0f, Vector2::GetZero());
	}

private:
	float Scale = 1.0f;
	Vector2 Translation;
};

constexpr SlateRenderTransform TransformCalculus2D::Concatenate(const SlateRenderTransform& Transform, const SlateLayoutTransform& Layout)
{
	return SlateRenderTransform(
		Matrix2x2::Multiply(Transform.GetMatrix(), Matrix2x2::Scale(Layout.GetScale())),
		Layout.TransformPoint(Transform.GetTranslation())
	);
}