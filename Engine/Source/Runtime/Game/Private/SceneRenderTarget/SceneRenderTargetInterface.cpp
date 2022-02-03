// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRenderTarget/SceneRenderTargetInterface.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHIRenderTargetView.h"
#include "RHI/IRHIDepthStencilView.h"
#include "RHI/IRHITexture2D.h"
#include "RHI/IRHIDeviceContext.h"

GENERATE_BODY(SSceneRenderTargetInterface);

SSceneRenderTargetInterface::SSceneRenderTargetInterface(IRHIDevice* InDevice, int32 InNumRTVs, bool bUseDSV) : Super()
{
	if (InNumRTVs)
	{
		RTV = InDevice->CreateRenderTargetView(InNumRTVs);
	}

	if (bUseDSV)
	{
		DSV = InDevice->CreateDepthStencilView(1);
	}
}

void SSceneRenderTargetInterface::SetRenderTargets(IRHITexture2D* InColorBuf, IRHITexture2D* InDepthStencilBuf, bool bBindResources)
{
	CleanupRenderTargets();

	if (InColorBuf)
	{
		RTV->CreateRenderTargetView(0, InColorBuf, nullptr);
	}

	if (InDepthStencilBuf)
	{
		DSV->CreateDepthStencilView(0, InDepthStencilBuf, nullptr);
	}

	this->bBindResources = bBindResources;
}

void SSceneRenderTargetInterface::CleanupRenderTargets()
{
	if (bBindResources)
	{
		if (RTV)
		{
			int32 Count = RTV->GetViewCount();
			for (int32 i = 0; i < Count; ++i)
			{
				if (IRHIResource* R = RTV->GetResource(i))
				{
					R->Dispose();
				}
			}
		}

		if (DSV)
		{
			if (IRHIResource* R = DSV->GetResource(0))
			{
				R->Dispose();
			}
		}
	}

	bBindResources = false;
}

void SSceneRenderTargetInterface::TransitRenderTargetViews(IRHIDeviceContext* DeviceContext, bool bTransitToRender)
{
	RHIResourceTransitionBarrier TransitionBarrier = {};
	TransitionBarrier.pResource = GetRenderTexture();
	TransitionBarrier.StateBefore = InitialState;
	TransitionBarrier.StateAfter = ERHIResourceStates::RenderTarget;

	if (!bTransitToRender)
	{
		std::swap(TransitionBarrier.StateBefore, TransitionBarrier.StateAfter);
	}

	DeviceContext->ResourceBarrier(TransitionBarrier);
}

void SSceneRenderTargetInterface::ClearRenderTargetViews(IRHIDeviceContext* DeviceContext)
{
	DeviceContext->ClearRenderTargetView(RTV, 0, NamedColors::Transparent);
}

void SSceneRenderTargetInterface::CopyRenderTargetOutput(IRHIDeviceContext* DeviceContext, IRHITexture2D* ColorOutput)
{
	if (bMultisampled)
	{
		DeviceContext->ResolveSubresource(ColorOutput, 0, GetRenderTexture(), 0, RenderTextureFormat);
	}
	else
	{
		DeviceContext->CopyResource(ColorOutput, GetRenderTexture());
	}
}

IRHITexture2D* SSceneRenderTargetInterface::GetRenderTexture()
{
	checkf(RTV->GetViewCount() == 1, L"ViewCount is not 1. Override this function to be your desired.");
	IRHIResource* PrimaryResource = RTV->GetResource(0);
	checkf(PrimaryResource, L"ViewResource is not binded to render target view. Override this function to be your desired.");
	auto* PrimaryTexture = Cast<IRHITexture2D>(PrimaryResource);
	checkf(PrimaryTexture, L"PrimaryViewResource of binded resource to render target view is not a IRHITexture2D type.");
	return PrimaryTexture;
}

IRHIRenderTargetView* SSceneRenderTargetInterface::GetRTV()
{
	return RTV;
}

IRHIDepthStencilView* SSceneRenderTargetInterface::GetDSV()
{
	return DSV;
}