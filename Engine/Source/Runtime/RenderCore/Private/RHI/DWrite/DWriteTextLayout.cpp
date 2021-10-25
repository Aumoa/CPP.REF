// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DWriteTextLayout.h"
#include "RHI/D3D12/D3D12Device.h"
#include "RHI/D3D12/D3D12CommandQueue.h"

SDWriteTextLayout::SDWriteTextLayout(SDXGIFactory* factory, SD3D12Device* device, ComPtr<IDWriteTextLayout> layout) : Super(factory, device, nullptr)
	, _layout(std::move(layout))
{
}

Vector2 SDWriteTextLayout::GetMaxSize()
{
	return Vector2(_layout->GetMaxWidth(), _layout->GetMaxHeight());
}

void SDWriteTextLayout::AllocateBuffer(const Vector2N& required)
{
	const bool bNeedRefresh = _requiredSize.X < required.X || _requiredSize.Y < required.Y;
	if (bNeedRefresh)
	{
		IRHIDeviceContext* deviceContext = _device->GetImmediateContext();

		// Add pending garbage object.
		struct __PendingObject
		{
			ComPtr<ID3D12Resource> _1;
			ComPtr<ID3D11Resource> _2;
			ComPtr<ID2D1Bitmap1> _3;
		} pendingObject = { std::move(_resource), std::move(_wrapped), std::move(_bitmap) };
		deviceContext->PendingGarbageObject(Cast<SObject>(std::move(pendingObject)));

		// Refresh required size.
		_requiredSize.X = AlignAs128(required.X);
		_requiredSize.Y = AlignAs128(required.Y);

		// Create D3D12 texture resource.
		auto* device = _device->Get<ID3D12Device>();
		D3D12_RESOURCE_DESC textureDesc =
		{
			.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
			.Width = (UINT64)_requiredSize.X,
			.Height = (UINT32)_requiredSize.Y,
			.DepthOrArraySize = 1,
			.MipLevels = 1,
			.Format = DXGI_FORMAT_A8_UNORM,
			.SampleDesc = { 1, 0 },
			.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
		};

		D3D12_CLEAR_VALUE clearValue =
		{
			.Format = textureDesc.Format,
			.Color = {}
		};

		D3D12_HEAP_PROPERTIES heapProp = { .Type = D3D12_HEAP_TYPE_DEFAULT };
		HR(device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &textureDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue, IID_PPV_ARGS(&_resource)));

		// Create interop resource for Direct2D.
		auto* deviceInt = _device->Get<ID3D11On12Device>();
		D3D11_RESOURCE_FLAGS flags = { D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE };
		HR(deviceInt->CreateWrappedResource(_resource.Get(), &flags, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, IID_PPV_ARGS(&_wrapped)));

		ComPtr<IDXGISurface> dxgiSurf;
		HR(_wrapped.As(&dxgiSurf));

		auto dc = _device->Get<ID2D1DeviceContext>();
		HR(dc->CreateBitmapFromDxgiSurface(dxgiSurf.Get(), D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW, D2D1::PixelFormat(DXGI_FORMAT_A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED), 96.0f, 96.0f, nullptr), &_bitmap));

		_bNeedRender = true;
	}
}

constexpr int32 SDWriteTextLayout::AlignAs128(int32 required)
{
	return (required + 127) & ~127;
}