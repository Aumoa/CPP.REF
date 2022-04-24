// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Multimedia/IPlatformImage.h"
#include "WindowsIncludes.h"

namespace libty::inline Windows
{
	class SWindowsImage : implements(SObject, IPlatformImage)
	{
		GENERATED_BODY(SWindowsImage)

	private:
		ComPtr<IWICBitmapSource> ImageSource;
		ERHIPixelFormat Format;

	public:
		SWindowsImage(IWICBitmapSource* ImageSource, ERHIPixelFormat Format);

		virtual void Dispose() override;

		virtual void CopyPixels(const RectN* CopyRect, int32 Stride, int32 BufferSize, void* OutBuffer) override;
		virtual Vector2N GetPixelSize() override;
		virtual ERHIPixelFormat GetPixelFormat() override;

	protected:
		virtual void Dispose(bool bDisposing) override;
	};
}