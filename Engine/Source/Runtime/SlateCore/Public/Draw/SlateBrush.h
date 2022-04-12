// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "RHI/RHIInterfaces.h"

namespace libty::inline SlateCore
{
	struct SlateBrush
	{
		GENERATED_BODY(SlateBrush);

	public:
		//SPROPERTY(ImageSource)
		//IRHIShaderResourceView* ImageSource = nullptr;
		//Vector2 ImageSize = Vector2::Zero();

		SlateBrush(IRHIShaderResourceView* ImageSource, const Vector2& ImageSize)
			//: ImageSource(ImageSource)
			//, ImageSize(ImageSize)
		{
		}

		//std::wstring ToString(std::wstring_view InFormatArgs = L"") const
		//{
		//	return String::Format(L"DesiredSize: {}", ImageSize.ToString(InFormatArgs));
		//}

		//auto operator <=>(const SlateBrush& Rhs) const
		//{
		//	auto Compare = ImageSource <=> Rhs.ImageSource;
		//	if (Compare != 0)
		//	{
		//		return Compare;
		//	}

		//	Compare = ImageSize <=> Rhs.ImageSize;
		//	return Compare;
		//}

		//bool operator ==(const SlateBrush& Rhs) const
		//{
		//	return ImageSource == Rhs.ImageSource
		//		&& ImageSize == Rhs.ImageSize;
		//}
	};
}