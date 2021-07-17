// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"

class RHIShaderResourceView;

struct SlateBrush
{
	RHIShaderResourceView* ImageSource = nullptr;
	Vector2 ImageSize;

	SlateBrush()
	{
	}

	SlateBrush(RHIShaderResourceView* imageSource, const Vector2& imageSize)
		: ImageSource(imageSource)
		, ImageSize(imageSize)
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