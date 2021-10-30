// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "WindowsIncludes.h"
#include "WindowsDevice.h"
#include "WindowsSolidColorBrush.h"
#include "WindowsStrokeStyle.h"

SWindowsDevice::SWindowsDevice(HWND hWnd) : Super()
	, hWnd(hWnd)
{
	DXGI_SWAP_CHAIN_DESC SwapChainDesc =
	{
		.BufferDesc =
		{
			.RefreshRate = { .Numerator = 1, .Denominator = 0 },
			.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
			.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
			.Scaling = DXGI_MODE_SCALING_UNSPECIFIED
		},
		.SampleDesc =
		{
			.Count = 4,
			.Quality = 0
		},
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = 2,
		.OutputWindow = hWnd,
		.Windowed = true,
		.SwapEffect = DXGI_SWAP_EFFECT_DISCARD,
	};

	HR(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, nullptr, 0, D3D11_SDK_VERSION, &SwapChainDesc, &SwapChain, &D3DDevice, nullptr, &D3DContext));

	ComPtr<IDXGIDevice> DxgiDevice;
	HR(D3DDevice.As(&DxgiDevice));
	HR(D2D1CreateDevice(DxgiDevice.Get(), D2D1::CreationProperties(D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_NONE, D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS), &D2DDevice));
	HR(D2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &D2DContext));

	D2DDevice->GetFactory(&D2DFactory);
}

void SWindowsDevice::Present()
{
	HR(SwapChain->Present(1, 0));
}

void SWindowsDevice::ResizeBuffers(const Vector2N& InPixelSize)
{
	SwapChainBuf.Reset();
	SwapChainBitmap.Reset();

	HR(SwapChain->ResizeBuffers(0, (UINT)InPixelSize.X, (UINT)InPixelSize.Y, DXGI_FORMAT_UNKNOWN, 0));

	HR(SwapChain->GetBuffer(0, IID_PPV_ARGS(&SwapChainBuf)));
	ComPtr<IDXGISurface> DxgiSurf;
	HR(SwapChainBuf.As(&DxgiSurf));

	float DpiScale = (float)GetDpiForWindow(hWnd);
	HR(D2DContext->CreateBitmapFromDxgiSurface(DxgiSurf.Get(), D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_CANNOT_DRAW | D2D1_BITMAP_OPTIONS_TARGET, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED), DpiScale, DpiScale), &SwapChainBitmap));
}

IRHISolidColorBrush* SWindowsDevice::CreateSolidColorBrush(const Color& InColor, float InOpacity, const SlateRenderTransform& InTransform)
{
	ComPtr<ID2D1SolidColorBrush> Brush;
	HR(D2DContext->CreateSolidColorBrush(GetColor(InColor), D2D1::BrushProperties(InOpacity, GetRenderTransform(InTransform)), &Brush));
	return NewObject<SWindowsSolidColorBrush>(Brush.Get());
}

IRHIStrokeStyle* SWindowsDevice::CreateStrokeStyle(const RHIStrokeStyleDesc& Desc, const std::vector<float>& CustomDashes)
{
	ComPtr<ID2D1StrokeStyle> Style;
	HR(D2DFactory->CreateStrokeStyle(reinterpret_cast<const D2D1_STROKE_STYLE_PROPERTIES*>(&Desc), CustomDashes.size() == 0 ? nullptr : CustomDashes.data(), (UINT32)CustomDashes.size(), &Style));
	return NewObject<SWindowsStrokeStyle>(Style.Get());
}

void SWindowsDevice::BeginDraw(const Color& ClearColor)
{
	D2DContext->SetTarget(SwapChainBitmap.Get());
	D2DContext->BeginDraw();
	D2DContext->Clear(D2D1::ColorF(ClearColor.R, ClearColor.G, ClearColor.B, ClearColor.A));
}

void SWindowsDevice::EndDraw()
{
	HR(D2DContext->EndDraw());
	D2DContext->SetTarget(nullptr);
}

void SWindowsDevice::FillRectangle(IRHIBrush* InBrush, Rect InRect)
{
	ID2D1Brush* Brush = Cast<SWindowsBrush>(InBrush)->GetBrush();
	D2DContext->FillRectangle(GetRect(InRect), Brush);
}

void SWindowsDevice::DrawRectangle(IRHIBrush* InBrush, Rect InRect, float StrokeWidth, IRHIStrokeStyle* StrokeStyle, ERHIStrokeDirection Direction)
{
	float HalfStroke;
	switch (Direction)
	{
	case ERHIStrokeDirection::Inner:
		HalfStroke = StrokeWidth * 0.5f;
		InRect.Left += HalfStroke;
		InRect.Top += HalfStroke;
		InRect.Right -= HalfStroke;
		InRect.Bottom -= HalfStroke;
		break;
	case ERHIStrokeDirection::Outer:
		HalfStroke = StrokeWidth * 0.5f;
		InRect.Left -= HalfStroke;
		InRect.Top -= HalfStroke;
		InRect.Right += HalfStroke;
		InRect.Bottom += HalfStroke;
		break;
	}

	ID2D1Brush* Brush = Cast<SWindowsBrush>(InBrush)->GetBrush();
	D2DContext->DrawRectangle(GetRect(InRect), Brush, StrokeWidth, StrokeStyle ? Cast<SWindowsStrokeStyle>(StrokeStyle)->GetStyle() : nullptr);
}

void SWindowsDevice::SetTransform(const SlateRenderTransform& InTransform)
{
	D2DContext->SetTransform(GetRenderTransform(InTransform));
}

SlateRenderTransform SWindowsDevice::GetTransform()
{
	D2D1_MATRIX_3X2_F T;
	D2DContext->GetTransform(&T);
	return GetRenderTransform(T);
}