// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SlateLayoutTransform.h"

struct LayoutGeometry
{
	constexpr LayoutGeometry()
		: LocalSize(1.0f, 1.0f)
	{
	}

	constexpr LayoutGeometry(const SlateLayoutTransform& localToParent, const Vector2& sizeInLocalSpace)
		: LocalToParent(localToParent)
		, LocalSize(sizeInLocalSpace)
	{
	}

	constexpr const SlateLayoutTransform& GetLocalToParentTransform() const
	{
		return LocalToParent;
	}

	constexpr const Vector2& GetSizeInLocalSpace() const
	{
		return LocalSize;
	}

	constexpr Vector2 GetSizeInParentSpace() const
	{
		return TransformCalculus2D::TransformVector(LocalToParent, LocalSize);
	}

	constexpr Vector2 GetOffsetInParentSpace() const
	{
		return LocalToParent.GetTranslation();
	}

	constexpr Rect GetRectInLocalSpace() const
	{
		return Rect(Vector2::GetZero(), LocalSize);
	}

	constexpr Rect GetRectInParentSpace() const
	{
		return TransformCalculus2D::TransformRect(LocalToParent, GetRectInLocalSpace());
	}

private:
	SlateLayoutTransform LocalToParent;
	Vector2 LocalSize;
};