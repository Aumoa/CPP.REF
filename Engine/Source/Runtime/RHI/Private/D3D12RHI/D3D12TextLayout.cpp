// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "D3D12RHI/D3D12TextLayout.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12TextFormat.h"
#include "D3D12RHI/D3D12Global.h"
#include "D3D12RHI/D3D12Texture2D.h"
#include "D3D12RHI/D3D12ShaderResourceView.h"
#include "Numerics/VectorInterface/Color.h"

ND3D12TextLayout::ND3D12TextLayout(std::shared_ptr<ND3D12TextFormat> InTextFormat, ComPtr<IDWriteTextLayout> pInTextLayout, String InText)
	: TextFormat(std::move(InTextFormat))
	, pTextLayout(std::move(pInTextLayout))
	, CachedText(InText)
{
	auto& Graphics = ND3D12Global::GetDynamicRHI();
	pDeviceContext = Graphics.CreateDeviceContext2D();
	HR(pDeviceContext->CreateSolidColorBrush((D2D1_COLOR_F&)NamedColors::White, &pSolidBrush));

	TryCacheImage();
}

Vector2 ND3D12TextLayout::GetDrawingSize() const
{
	DWRITE_TEXT_METRICS Metrics;
	HR(pTextLayout->GetMetrics(&Metrics));
	return Vector2(Metrics.width, Metrics.height);
}

Rect ND3D12TextLayout::GetTextureCoordinate() const
{
	DWRITE_TEXT_METRICS Metrics;
	HR(pTextLayout->GetMetrics(&Metrics));
	double TexX = (double)Metrics.width / Math::Ceil((double)Metrics.width);
	double TexY = (double)Metrics.height / Math::Ceil((double)Metrics.height);
	return Rect(0.0f, 0.0f, (float)TexX, (float)TexY);
}

std::shared_ptr<NRHITextFormat> ND3D12TextLayout::GetTextFormat() const
{
	return TextFormat;
}

void ND3D12TextLayout::TryCacheImage()
{
	auto& Graphics = ND3D12Global::GetDynamicRHI();

	DWRITE_TEXT_METRICS Metrics;
	HR(pTextLayout->GetMetrics(&Metrics));
	Vector2N TextureSize = { (int32)Math::Ceil(Metrics.width), (int32)Math::Ceil(Metrics.height) };

	D3D12_RESOURCE_DESC Desc = GetTexture2DDesc(TextureSize, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, DXGI_FORMAT_A8_UNORM);
	HR(Graphics.GetDevice()->CreateCommittedResource(
		&DefaultHeap,
		D3D12_HEAP_FLAG_NONE,
		&Desc,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		nullptr,
		IID_PPV_ARGS(&pFontResource))
	);

	D3D11_RESOURCE_FLAGS ResourceFlags = { .BindFlags = D3D11_BIND_RENDER_TARGET };
	HR(Graphics.GetD3D11On12Device().CreateWrappedResource(pFontResource.Get(), &ResourceFlags, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, IID_PPV_ARGS(&pInteropResource)));

	D2D1_BITMAP_PROPERTIES1 BitmapProp =
	{
		.pixelFormat =
		{
			.format = DXGI_FORMAT_A8_UNORM,
			.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED
		},
		.dpiX = 96.0f,
		.dpiY = 96.0f,
		.bitmapOptions = D2D1_BITMAP_OPTIONS_CANNOT_DRAW | D2D1_BITMAP_OPTIONS_TARGET
	};

	ComPtr<IDXGISurface> pSurface;
	HR(pInteropResource.As(&pSurface));
	HR(pDeviceContext->CreateBitmapFromDxgiSurface(pSurface.Get(), BitmapProp, &pBitmap));

	TextureResource = std::make_shared<ND3D12Texture2D>(pFontResource, Desc);
	SRV = std::make_shared<ND3D12ShaderResourceView>(*Graphics.GetDevice(), 1);
	SRV->CreateView(0, TextureResource.get());

	ID3D11On12Device& InteropDev = Graphics.GetD3D11On12Device();
	InteropDev.AcquireWrappedResources(pInteropResource.GetAddressOf(), 1);

	pDeviceContext->SetTarget(pBitmap.Get());
	pDeviceContext->BeginDraw();
	pDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
	pDeviceContext->DrawTextLayout(D2D1::Point2F(), pTextLayout.Get(), pSolidBrush.Get());
	HR(pDeviceContext->EndDraw());
	pDeviceContext->SetTarget(nullptr);

	InteropDev.ReleaseWrappedResources(pInteropResource.GetAddressOf(), 1);
	Graphics.GetD3D11DeviceContext().Flush();
}

#endif