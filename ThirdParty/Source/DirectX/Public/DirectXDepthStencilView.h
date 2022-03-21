// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "DirectXView.h"

class DIRECTX_API SDirectXDepthStencilView : public SDirectXView, implements IRHIDepthStencilView
{
	GENERATED_BODY(SDirectXDepthStencilView)

public:
	SDirectXDepthStencilView(SDirectXDevice* Owner, ComPtr<ID3D12DescriptorHeap> pDescriptorHeap, size_t NumViews);

	virtual void CreateDepthStencilView(size_t index, IRHIResource* pResource, const RHIDepthStencilViewDesc* pDesc) override;
};