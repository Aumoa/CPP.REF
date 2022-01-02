// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Texture/Texture2D.h"

interface IRHIBitmap;
class STexture2D;

struct SlateBrush
{
	IRHIBitmap* ImageSource = nullptr;
	Vector2 ImageSize = Vector2::ZeroVector();

	SlateBrush(IRHIBitmap* InImageSource, const Vector2& InImageSize)
		: ImageSource(InImageSource)
		, ImageSize(InImageSize)
	{
	}

	SlateBrush(STexture2D* InTexture)
	{
		if (InTexture)
		{
			ImageSource = InTexture->GetBitmap();
			ImageSize = InTexture->GetSize().Cast<float>();
		}
	}

	SlateBrush(const Vector2& ImageSize = Vector2(128.0f, 128.0f))
		: ImageSize(ImageSize)
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