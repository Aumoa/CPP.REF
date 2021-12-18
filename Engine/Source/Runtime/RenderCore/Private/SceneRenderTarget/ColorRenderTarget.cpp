// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRenderTarget/ColorRenderTarget.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHITexture2D.h"
#include "RHI/IRHIRenderTargetView.h"
#include "RHI/IRHIBitmap.h"

GENERATE_BODY(SColorRenderTarget);

SColorRenderTarget::SColorRenderTarget(IRHIDevice* InDevice, const Vector2N& InitialViewportSize) : Super(InDevice, 1, false)
	, Device(InDevice)
{
	InitialState = ERHIResourceStates::ResolveSource;
	RenderTextureFormat = ERHIPixelFormat::B8G8R8A8_UNORM;
	SetViewportSize(InitialViewportSize);
}

IRHITexture2D* SColorRenderTarget::GetRenderTexture()
{
	return RenderTarget;
}

IRHIBitmap* SColorRenderTarget::GetRenderBitmap()
{
	return RenderBitmap;
}

void SColorRenderTarget::SetViewportSize(const Vector2N& InViewportSize)
{
	RHITexture2DClearValue ClearValue;
	ClearValue.Format = ERHIPixelFormat::B8G8R8A8_UNORM;
	reinterpret_cast<Color&>(*ClearValue.ClearColor) = NamedColors::Transparent;

	RHITexture2DDesc Desc = {};
	Desc.Width = InViewportSize.X;
	Desc.Height = InViewportSize.Y;
	Desc.DepthOrArraySize = 1;
	Desc.MipLevels = 1;
	Desc.Format = RenderTextureFormat;
	Desc.Usage = ERHIBufferUsage::Default;
	Desc.Flags = ERHIResourceFlags::AllowRenderTarget;
	Desc.InitialState = ERHIResourceStates::ResolveSource;
	Desc.ClearValue = ClearValue;
	Desc.SampleDesc = { 4, 0 };

	RenderTarget = Device->CreateTexture2D(Desc, nullptr);
	GetRTV()->CreateRenderTargetView(0, RenderTarget, nullptr);

	RenderBitmap = Device->CreateBitmapFromTexture2D(RenderTarget);
	bMultisampled = Desc.SampleDesc.Count != 1;
}