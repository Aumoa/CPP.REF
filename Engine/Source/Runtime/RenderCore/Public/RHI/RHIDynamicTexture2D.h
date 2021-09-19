// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once


#include "CoreMinimal.h"
#include "RHITexture2D.h"

class SRHIDeviceContext;
struct D3D12_PLACED_SUBRESOURCE_FOOTPRINT;

class RENDERCORE_API SRHIDynamicTexture2D : public SRHITexture2D
{
	GENERATED_BODY(SRHIDynamicTexture2D)

private:
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT _layout;
	ID3D12Resource* _texture = nullptr;
	ComPtr<ID3D12Resource> _uploadHeap = nullptr;
	void* _pointer = nullptr;

	SRHIDeviceContext* _deviceContext = nullptr;
	int32 _updateBoxLeft = -1, _updateBoxTop = 0, _updateBoxRight = 0, _updateBoxBottom = 0;

public:
	SRHIDynamicTexture2D(SRHIDevice* device, ID3D12Resource* texture, ID3D12Resource* uploadHeap, const D3D12_PLACED_SUBRESOURCE_FOOTPRINT& layout);
	virtual ~SRHIDynamicTexture2D() override;

	void CloseUploadBuffer();
	void UpdatePixels(const void* buffer, int32 pixelStride, const Vector2N& size, const Vector2N& location);
	void Apply();

private:
	void RefreshUpdateBox(int32 left, int32 top, int32 right, int32 bottom);
	void ResetUpdateBox();
	inline bool HasUpdateBox() const { return _updateBoxLeft != -1; }
};