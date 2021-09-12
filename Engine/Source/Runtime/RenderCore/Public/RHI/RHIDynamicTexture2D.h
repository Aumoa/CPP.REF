// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once


#include "CoreMinimal.h"
#include "RHITexture2D.h"

class RHIDeviceContext;
struct D3D12_PLACED_SUBRESOURCE_FOOTPRINT;

class RENDERCORE_API RHIDynamicTexture2D : public RHITexture2D
{
	GENERATED_BODY(RHIDynamicTexture2D)

private:
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT _layout;
	ID3D12Resource* _texture = nullptr;
	ComPtr<ID3D12Resource> _uploadHeap = nullptr;
	void* _pointer = nullptr;

	RHIDeviceContext* _deviceContext = nullptr;
	int32 _updateBoxLeft = -1, _updateBoxTop = 0, _updateBoxRight = 0, _updateBoxBottom = 0;

public:
	RHIDynamicTexture2D(RHIDevice* device, ID3D12Resource* texture, ID3D12Resource* uploadHeap, const D3D12_PLACED_SUBRESOURCE_FOOTPRINT& layout);
	virtual ~RHIDynamicTexture2D() override;

	void CloseUploadBuffer();
	void UpdatePixels(const void* buffer, int32 pixelStride, int32 sizeX, int32 sizeY, int32 locationX, int32 locationY);
	void Apply();

private:
	void RefreshUpdateBox(int32 left, int32 top, int32 right, int32 bottom);
	void ResetUpdateBox();
	inline bool HasUpdateBox() const { return _updateBoxLeft != -1; }
};