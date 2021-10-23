// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneRenderTarget.h"
#include "RHI/IRHIRenderTargetView.h"
#include "RHI/IRHIDepthStencilView.h"
#include "RHI/IRHITexture2D.h"
#include "RHI/RHIEnums.h"

SceneRenderTarget::SceneRenderTarget(IRHIRenderTargetView* InRTV, int32 IndexOfRTV, IRHIDepthStencilView* InDSV, int32 IndexOfDSV, ERHIResourceStates InInitState)
	: RTV(InRTV)
	, DSV(InDSV)
	, IndexOfRTV(IndexOfRTV)
	, IndexOfDSV(IndexOfDSV)
	, InitState(InInitState)
{
	IRHITexture2D* ReferencedTexture = nullptr;

	if (InRTV)
	{
		RTTexture = SObject::Cast<IRHITexture2D>(InRTV->GetResource(IndexOfRTV));
		ReferencedTexture = RTTexture;
	}

	if (InDSV)
	{
		DSTexture = SObject::Cast<IRHITexture2D>(InDSV->GetResource(IndexOfDSV));
		if (ReferencedTexture == nullptr)
		{
			ReferencedTexture = DSTexture;
		}
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
}