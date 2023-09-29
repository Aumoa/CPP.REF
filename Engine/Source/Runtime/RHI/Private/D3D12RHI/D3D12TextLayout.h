// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHITextLayout.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Common.h"

class ND3D12TextFormat;

class ND3D12TextLayout : public NRHITextLayout
{
	std::shared_ptr<ND3D12TextFormat> TextFormat;
	ComPtr<IDWriteTextLayout> pTextLayout;
	String CachedText;

	ComPtr<ID2D1DeviceContext> pDeviceContext;
	ComPtr<ID2D1SolidColorBrush> pSolidBrush;

	std::shared_ptr<NRHITexture2D> TextureResource;
	std::shared_ptr<NRHIShaderResourceView> SRV;
	ComPtr<ID3D12Resource> pFontResource;
	ComPtr<ID3D11Resource> pInteropResource;
	ComPtr<ID2D1Bitmap1> pBitmap;

public:
	ND3D12TextLayout(std::shared_ptr<ND3D12TextFormat> InTextFormat, ComPtr<IDWriteTextLayout> pInTextLayout, String InText);

	virtual String GetText() const override { return CachedText; }
	virtual Vector2 GetDrawingSize() const override;
	virtual Rect GetTextureCoordinate() const override;
	virtual std::shared_ptr<NRHITextFormat> GetTextFormat() const;

	virtual std::shared_ptr<NRHITexture2D> GetTexture2D() const override { return TextureResource; }
	virtual std::shared_ptr<NRHIShaderResourceView> GetSRV() const override { return SRV; }

	inline IDWriteTextLayout* Get() const { return pTextLayout.Get(); }

private:
	void TryCacheImage();
};

#endif