// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRenderTarget/ColorRenderTarget.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHITexture2D.h"
#include "RHI/IRHIRenderTargetView.h"

SColorRenderTarget::SColorRenderTarget(IRHIDevice* InDevice, const Vector2N& InitialViewportSize) : Super(InDevice, 1, false)
	, Device(InDevice)
{
	SetViewportSize(InitialViewportSize);
}

IRHITexture2D* SColorRenderTarget::GetRenderTexture()
{
	return RenderTarget;
}

void SColorRenderTarget::SetViewportSize(const Vector2N& InViewportSize)
{
	if (RenderTarget && RenderTarget->GetOuter() == this)
	{
		DestroyObject(RenderTarget);
	}

	RHITexture2DClearValue ClearValue;
	ClearValue.Format = ERHIPixelFormat::B8G8R8A8_UNORM;
	reinterpret_cast<Color&>(*ClearValue.ClearColor) = NamedColors::Transparent;

	RHITexture2DDesc Desc = {};
	Desc.Width = InViewportSize.X;
	Desc.Height = InViewportSize.Y;
	Desc.DepthOrArraySize = 1;
	Desc.MipLevels = 1;
	Desc.Format = ERHIPixelFormat::B8G8R8A8_UNORM;
	Desc.Usage = ERHIBufferUsage::Default;
	Desc.Flags = ERHIResourceFlags::AllowRenderTarget;
	Desc.InitialState = ERHIResourceStates::PixelShaderResource | ERHIResourceStates::CopySource;
	Desc.ClearValue = ClearValue;

	RenderTarget = Device->CreateTexture2D(Desc, nullptr);
	GetRTV()->CreateRenderTargetView(0, RenderTarget, nullptr);
}