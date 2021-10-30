// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHITexture2D;

struct SlateBrush
{
	IRHITexture2D* ImageSource = nullptr;
	Vector2 ImageSize;
	Color TintColor;

	SlateBrush()
	{
	}

	SlateBrush(IRHITexture2D* InImageSource, const Vector2& InImageSize, const Color& InTintColor)
		: ImageSource(InImageSource)
		, ImageSize(InImageSize)
		, TintColor(InTintColor)
	{
	}

	bool operator ==(const SlateBrush& Rhs) const
	{
		return ImageSource == Rhs.ImageSource
			&& ImageSize == Rhs.ImageSize
			&& TintColor == Rhs.TintColor;
	}

	bool operator !=(const SlateBrush& Rhs) const
	{
		return ImageSource != Rhs.ImageSource
			|| ImageSize != Rhs.ImageSize
			|| TintColor != Rhs.TintColor;
	}
};