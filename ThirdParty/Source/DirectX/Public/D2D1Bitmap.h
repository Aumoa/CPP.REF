// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D2D1Image.h"
#include "RHI/IRHIBitmap.h"

class DIRECTX_API SD2D1Bitmap : public SD2D1Image, implements IRHIBitmap
{
	GENERATED_BODY(SD2D1Bitmap)

private:
	ComPtr<ID3D12Resource> Resource;
	ComPtr<ID3D11Resource> WrappedResource;
	ID2D1Bitmap1* Bitmap = nullptr;

public:
	SD2D1Bitmap(SDXGIFactory* InFactory, SD3D12Device* InDevice, ID2D1Bitmap1* Bitmap);

	virtual Vector2N GetSize() override;
	
	void InitWrappedResources(ID3D12Resource* pResource, ID3D11Resource* pWrapped);
	bool IsMultisampled();

protected:
	virtual void Dispose(bool bDisposing) override;

public:
	DECLARE_GETTER(ID2D1Bitmap1, Bitmap);
	DECLARE_GETTER(ID3D11Resource, WrappedResource);
	DECLARE_GETTER(ID3D12Resource, Resource);
};