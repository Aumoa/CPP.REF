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

	constexpr SlateLayoutTransform(float scale, const Vector2& translation)
		: Scale(scale)
		, Translation(translation)
	{
	}

	constexpr SlateLayoutTransform(const Vector2& translation)
		: Scale(1.0f)
		, Translation(translation)
	{
	}

	std::wstring ToString(std::wstring_view formatArgs = L"") const
	{
		return std::format(L"Translation: {}, Scale: {}", Translation.ToString(formatArgs), std::format(StringUtils::GetPlaceholder(formatArgs), Scale));
	}

	template<class T>
	constexpr T TransformPoint(const T& point) const
	{
		return TransformCalculus2D::TransformPoint(Translation, Scale2D(Scale).TransformPoint(point));
	}

	template<class T>
	constexpr T TransformVector(const T& point) const
	{
		return TransformCalculus2D::TransformVector(Translation, Scale2D(Scale).TransformVector(point));
	}

	constexpr SlateLayoutTransform Concatenate(const SlateLayoutTransform& rhs) const
	{
		return SlateLayoutTransform(TransformCalculus2D::Concatenate(Scale, rhs.Scale), rhs.TransformPoint(Translation));
	}

	constexpr SlateLayoutTransform Inverse() const
	{
		float invScale = 1.0f / Scale;
		return SlateLayoutTransform(invScale, -Translation * invScale);
	}

	constexpr bool operator ==(const SlateLayoutTransform& rhs) const
	{
		return Scale == rhs.Scale && Translation == rhs.Translation;
	}

	constexpr bool operator !=(const SlateLayoutTransform& rhs) const
	{
		return Scale != rhs.Scale || Translation != rhs.Translation;
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

constexpr SlateRenderTransform TransformCalculus2D::Concatenate(const SlateRenderTransform& transform, const SlateLayoutTransform& layout)
{
	return SlateRenderTransform(
		Matrix2x2::Multiply(transform.GetMatrix(), Matrix2x2::Scale(layout.GetScale())),
		layout.TransformPoint(transform.GetTranslation())
	);
}