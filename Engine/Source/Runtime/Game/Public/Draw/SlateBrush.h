// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIRenderTargetView;

struct SlateBrush
{
	IRHIRenderTargetView* ImageSource = nullptr;
	Vector2 ImageSize;

	SlateBrush()
	{
	}

	SlateBrush(IRHIRenderTargetView* imageSource, const Vector2& imageSize)
		: ImageSource(imageSource)
		, ImageSize(imageSize)
	{
	}

	template<class TSource>
	SlateBrush(const TSource* textureSource)
		: ImageSource(textureSource->GetShaderResourceView())
		, ImageSize(textureSource->GetPixelSize().Cast<float>())
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