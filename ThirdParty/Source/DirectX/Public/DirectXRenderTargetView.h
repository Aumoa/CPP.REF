// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DirectXView.h"

class DIRECTX_API SDirectXRenderTargetView : public SDirectXView, implements IRHIRenderTargetView
{
	GENERATED_BODY(SDirectXRenderTargetView)

public:
	SDirectXRenderTargetView(SDirectXDevice* Owner, ComPtr<ID3D12DescriptorHeap> pDescriptorHeap, size_t NumViews);

	virtual void CreateRenderTargetView(int32 Index, IRHIResource* Resource, const RHIRenderTargetViewDesc* Desc) override;
};