// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIShaderResourceView;

struct SlateBrush
{
	IRHIShaderResourceView* ImageSource = nullptr;
	Vector2 ImageSize;

	SlateBrush()
	{
	}

	SlateBrush(IRHIShaderResourceView* InImageSource, const Vector2& InImageSize)
		: ImageSource(InImageSource)
		, ImageSize(InImageSize)
	{
	}

	std::wstring ToString(std::wstring_view formatArgs = L"") const
	{
		return std::format(L"DesiredSize: {}", ImageSize.ToString(formatArgs));
	}

	bool operator ==(const SlateBrush& rhs) const
	{
		return ImageSource == rhs.ImageSource
			&& ImageSize == rhs.ImageSize;
	}

	bool operator !=(const SlateBrush& rhs) const
	{
		return ImageSource != rhs.ImageSource
			|| ImageSize != rhs.ImageSize;
	}
};