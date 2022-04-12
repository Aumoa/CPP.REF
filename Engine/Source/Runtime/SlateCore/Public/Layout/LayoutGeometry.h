// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "SlateLayoutTransform.h"

namespace libty::inline SlateCore
{
	struct LayoutGeometry
	{
		constexpr LayoutGeometry()
			: LocalSize(1.0f, 1.0f)
		{
		}

		constexpr LayoutGeometry(const SlateLayoutTransform& LocalToParent, const Vector2& SizeInLocalSpace)
			: LocalToParent(LocalToParent)
			, LocalSize(SizeInLocalSpace)
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
		SlateLayoutTransform LocalToParent;
		Vector2 LocalSize;
	};
}