// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/SlateLayoutTransform.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Numerics/VectorInterface/Rect.h"

struct NLayoutGeometry
{
	constexpr NLayoutGeometry()
		: LocalSize(1.0f, 1.0f)
	{
	}

	constexpr NLayoutGeometry(const NSlateLayoutTransform& LocalToParent, const Vector2& SizeInLocalSpace)
		: LocalToParent(LocalToParent)
		, LocalSize(SizeInLocalSpace)
	{
	}

	constexpr const NSlateLayoutTransform& GetLocalToParentTransform() const
	{
		return LocalToParent;
	}

	constexpr const Vector2& GetSizeInLocalSpace() const
	{
		return LocalSize;
	}

	constexpr Vector2 GetSizeInParentSpace() const
	{
		return LocalToParent.TransformVector(LocalSize);
	}

	constexpr Vector2 GetOffsetInParentSpace() const
	{
		return LocalToParent.Translation;
	}

	constexpr Rect GetRectInLocalSpace() const
	{
		return Rect(Vector2::Zero(), LocalSize);
	}

	constexpr Rect GetRectInParentSpace() const
	{
		return Rect::TransformRect(LocalToParent, GetRectInLocalSpace());
	}

private:
	NSlateLayoutTransform LocalToParent;
	Vector2 LocalSize;
};