// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/IRHIDevice.h"
#include "WindowsIncludes.h"

struct SlateDrawElement;

class SWindowsDevice : implements SObject, implements IRHIDevice
{
	GENERATED_BODY(SWindowsDevice)

private:
	HWND hWnd = nullptr;

	ComPtr<ID3D11Device> D3DDevice;
	ComPtr<ID3D11DeviceContext> D3DContext;
	ComPtr<IDXGISwapChain> SwapChain;

	ComPtr<ID3D11Texture2D> SwapChainBuf;
	ComPtr<ID2D1Bitmap1> SwapChainBitmap;

	ComPtr<ID2D1Factory> D2DFactory;
	ComPtr<ID2D1Device> D2DDevice;
	ComPtr<ID2D1DeviceContext> D2DContext;

public:
	SWindowsDevice(HWND hWnd);

	virtual void Present() override;
	virtual void ResizeBuffers(const Vector2N& InPixelSize) override;

	virtual IRHISolidColorBrush* CreateSolidColorBrush(const Color& InColor, float InOpacity, const SlateRenderTransform& InTransform) override;
	virtual IRHIStrokeStyle* CreateStrokeStyle(const RHIStrokeStyleDesc& Desc, const std::vector<float>& CustomDashes) override;

	virtual void BeginDraw(const Color& ClearColor) override;
	virtual void EndDraw() override;
	virtual void FillRectangle(IRHIBrush* InBrush, Rect InRect) override;
	virtual void DrawRectangle(IRHIBrush* InBrush, Rect InRect, float StrokeWidth, IRHIStrokeStyle* StrokeStyle, ERHIStrokeDirection Direction) override;
	virtual void SetTransform(const SlateRenderTransform& InTransform) override;
	virtual SlateRenderTransform GetTransform() override;
};