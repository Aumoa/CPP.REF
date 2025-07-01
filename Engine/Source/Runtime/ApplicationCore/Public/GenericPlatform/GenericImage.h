// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Rect.h"
#include "GenericImage.gen.h"

namespace Ayla
{
	ACLASS()
	class APPLICATIONCORE_API GenericImage : public Object
	{
		GENERATED_BODY()

	private:
		void* pImpl = nullptr;

	private:
		GenericImage();

	public:
		~GenericImage() noexcept;

		virtual void CopyPixels(int32 Stride, int32 BufferSize, void* DestBuf) = 0;
		virtual Task<> CopyPixelsAsync(int32 Stride, int32 BufferSize, void* DestBuf) = 0;
		virtual void CopyPixels(const RectN& InCopyRect, int32 Stride, int32 BufferSize, void* DestBuf) = 0;
		virtual Task<> CopyPixelsAsync(RectN InCopyRect, int32 Stride, int32 BufferSize, void* DestBuf) = 0;

		virtual Vector2N GetSize() const = 0;

		static Task<RPtr<GenericImage>> LoadFromFileAsync(String InFilename);
	};
}