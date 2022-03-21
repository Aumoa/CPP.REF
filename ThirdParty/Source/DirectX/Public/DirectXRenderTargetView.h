// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "DirectXView.h"

class DIRECTX_API SDirectXRenderTargetView : public SDirectXView, implements IRHIRenderTargetView
{
	GENERATED_BODY(SDirectXRenderTargetView)

public:
	SDirectXRenderTargetView(SDirectXDevice* Owner, ComPtr<ID3D12DescriptorHeap> pDescriptorHeap, size_t NumViews);

	virtual void CreateRenderTargetView(size_t index, IRHIResource* pResource, const RHIRenderTargetViewDesc* pDesc) override;
};