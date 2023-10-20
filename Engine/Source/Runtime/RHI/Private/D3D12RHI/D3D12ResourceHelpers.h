// Copyright 2020-2023 Aumoa.lib. All right reserved.

#ifdef __D3D12_RESOURCE_HELPERS_ASSERT
#error Do NOT include D3D12ResourceHelpers manually. Include D3D12Common.h instead.
#endif

#define __D3D12_RESOURCE_HELPERS_ASSERT

#include "Numerics/VectorInterface/Vector.h"

inline D3D12_RESOURCE_DESC GetTexture2DDesc(const Vector2N& TextureSize, D3D12_RESOURCE_FLAGS Flags, DXGI_FORMAT InFormat = DXGI_FORMAT_R8G8B8A8_UNORM)
{
	return
	{
		.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		.Alignment = 0,
		.Width = (UINT64)TextureSize.X,
		.Height = (UINT)TextureSize.Y,
		.DepthOrArraySize = 1,
		.MipLevels = 1,
		.Format = InFormat,
		.SampleDesc = { 1, 0 },
		.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
		.Flags = Flags
	};
}

inline D3D12_RESOURCE_DESC GetBufferDesc(int64 BufferSize)
{
	return
	{
		.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
		.Alignment = 0,
		.Width = (UINT64)BufferSize,
		.Height = 1,
		.DepthOrArraySize = 1,
		.MipLevels = 1,
		.Format = DXGI_FORMAT_UNKNOWN,
		.SampleDesc = { 1, 0 },
		.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
		.Flags = D3D12_RESOURCE_FLAG_NONE
	};
}

inline static D3D12_HEAP_PROPERTIES DefaultHeap = { D3D12_HEAP_TYPE_DEFAULT };
inline static D3D12_HEAP_PROPERTIES UploadHeap = { D3D12_HEAP_TYPE_UPLOAD };