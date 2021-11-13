// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIBitmap;

struct SlateBrush
{
	IRHIBitmap* ImageSource = nullptr;
	Vector2 ImageSize = Vector2::ZeroVector();
	Color TintColor = NamedColors::White;

	SlateBrush()
	{
	}

	SlateBrush(IRHIBitmap* InImageSource, const Vector2& InImageSize)
		: ImageSource(InImageSource)
		, ImageSize(InImageSize)
	{
	}

	SlateBrush(const Color& TintColor)
		: TintColor(TintColor)
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