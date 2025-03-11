// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Rect.h"

class APPLICATIONCORE_API NGenericImage : public std::enable_shared_from_this<NGenericImage>
{
private:
	void* pImpl = nullptr;

private:
	NGenericImage();

public:
	~NGenericImage() noexcept;

	void CopyPixels(int32 Stride, int32 BufferSize, void* DestBuf);
	Task<> CopyPixelsAsync(int32 Stride, int32 BufferSize, void* DestBuf);
	void CopyPixels(const RectN& InCopyRect, int32 Stride, int32 BufferSize, void* DestBuf);
	Task<> CopyPixelsAsync(RectN InCopyRect, int32 Stride, int32 BufferSize, void* DestBuf);

	Vector2N GetSize() const;

	static Task<std::shared_ptr<NGenericImage>> LoadFromFileAsync(String InFilename);
};