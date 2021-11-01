// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneRenderTarget.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHIRenderTargetView.h"
#include "RHI/IRHIDepthStencilView.h"
#include "RHI/IRHITexture2D.h"

SSceneRenderTarget::SSceneRenderTarget(IRHIDevice* InDevice, int32 InNumRTVs, bool bUseDSV) : Super()
{
	if (InNumRTVs)
	{
		RTV = InDevice->CreateRenderTargetView(InNumRTVs);
		RTV->SetOuter(this);
	}

	if (bUseDSV)
	{
		DSV = InDevice->CreateDepthStencilView(1);
		DSV->SetOuter(this);
	}
}

void SSceneRenderTarget::SetRenderTargets(IRHITexture2D* InColorBuf, IRHITexture2D* InDepthStencilBuf, bool bBindResources)
{
	CleanupRenderTargets();

	if (InColorBuf)
	{
		check(RTV);
		RTV->CreateRenderTargetView(0, InColorBuf, nullptr);

		if (bBindResources)
		{
			InColorBuf->SetOuter(this);
		}
	}

	if (InDepthStencilBuf)
	{
		check(DSV);
		DSV->CreateDepthStencilView(0, InDepthStencilBuf, nullptr);

		if (bBindResources)
		{
			InDepthStencilBuf->SetOuter(this);
		}
	}

	this->bBindResources = bBindResources;
}

void SSceneRenderTarget::CleanupRenderTargets()
{
	if (bBindResources)
	{
		if (RTV)
		{
			int32 Count = RTV->GetViewCount();
			for (int32 i = 0; i < Count; ++i)
			{
				if (IRHIResource* R = RTV->GetResource(i); R && R->GetOuter() == this)
				{
					DestroyObject(R);
				}
			}
		}

		if (DSV)
		{
			if (IRHIResource* R = DSV->GetResource(0); R && R->GetOuter() == this)
			{
				DestroyObject(R);
			}
		}
	}

	bBindResources = false;
}