// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHITexture2D;

struct SlateBrush
{
	IRHITexture2D* ImageSource = nullptr;
	Vector2 ImageSize;

	SlateBrush()
	{
	}

	SlateBrush(IRHITexture2D* InImageSource, const Vector2& InImageSize)
		: ImageSource(InImageSource)
		, ImageSize(InImageSize)
	{
	}

	std::wstring ToString(std::wstring_view InFormatArgs = L"") const
	{
		return std::format(L"DesiredSize: {}", ImageSize.ToString(InFormatArgs));
	}

	bool operator ==(const SlateBrush& Rhs) const
	{
		return ImageSource == Rhs.ImageSource
			&& ImageSize == Rhs.ImageSize;
	}

	bool operator !=(const SlateBrush& Rhs) const
	{
		return ImageSource != Rhs.ImageSource
			|| ImageSize != Rhs.ImageSize;
	}
};