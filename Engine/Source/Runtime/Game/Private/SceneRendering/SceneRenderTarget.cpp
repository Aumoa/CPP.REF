// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneRenderTarget.h"
#include "RHI/IRHIRenderTargetView.h"
#include "RHI/IRHIDepthStencilView.h"
#include "RHI/IRHITexture2D.h"
#include "RHI/RHIEnums.h"

GENERATE_BODY(SSceneRenderTarget);

SSceneRenderTarget::SSceneRenderTarget()
{
}

SSceneRenderTarget::SSceneRenderTarget(IRHIRenderTargetView* InRTV, int32 IndexOfRTV, IRHIDepthStencilView* InDSV, int32 IndexOfDSV, ERHIResourceStates InInitState)
{
	InitTarget(InRTV, IndexOfRTV, InDSV, IndexOfDSV, InInitState);
}

SSceneRenderTarget::~SSceneRenderTarget()
{
}

void SSceneRenderTarget::InitTarget(IRHIRenderTargetView* InRTV, int32 IndexOfRTV, IRHIDepthStencilView* InDSV, int32 IndexOfDSV, ERHIResourceStates InInitState)
{
	this->RTV = InRTV;
	this->DSV = InDSV;
	this->IndexOfRTV = IndexOfRTV;
	this->IndexOfDSV = IndexOfDSV;
	this->InitState = InInitState;

	IRHITexture2D* ReferencedTexture = nullptr;

	if (InRTV)
	{
		RTTexture = Cast<IRHITexture2D>(InRTV->GetResource(IndexOfRTV));
		ReferencedTexture = RTTexture;
	}
	else
	{
		RTV = nullptr;
		RTTexture = nullptr;
	}

	if (InDSV)
	{
		DSTexture = Cast<IRHITexture2D>(InDSV->GetResource(IndexOfDSV));
		if (ReferencedTexture == nullptr)
		{
			ReferencedTexture = DSTexture;
		}
	}
	else
	{
		DSV = nullptr;
		DSTexture = nullptr;
	}

	if (ReferencedTexture)
	{
		RHITexture2DDesc Desc = ReferencedTexture->GetDesc();

		// Setup viewport automatically.
		Viewport.TopLeftX = 0;
		Viewport.TopLeftY = 0;
		Viewport.Width = (float)Desc.Width;
		Viewport.Height = (float)Desc.Height;
		Viewport.MinDepth = 0.0f;
		Viewport.MaxDepth = 1.0f;

		// Setup scissor rect automatically.
		ScissorRect.Left = 0;
		ScissorRect.Top = 0;
		ScissorRect.Right = (int32)Desc.Width;
		ScissorRect.Bottom = (int32)Desc.Height;
	}

	bHasBeenTransited = false;
}