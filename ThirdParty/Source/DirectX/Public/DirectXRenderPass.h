// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DirectXDeviceChild.h"

class DIRECTX_API SDirectXRenderPass : public SDirectXDeviceChild, implements IRHIRenderPass
{
	GENERATED_BODY(SDirectXRenderPass)

public:
	std::vector<D3D12_RENDER_PASS_RENDER_TARGET_DESC> RenderTargets;
	std::vector<D3D12_RESOURCE_BARRIER> TransitionBarriers;

public:
	SDirectXRenderPass(SDirectXDevice* Owner);

	virtual void AddColorAttachment(const RHIRenderPassAttachment& AttachDesc) override;
	virtual void Apply(std::span<std::pair<IRHIView*, int32> const> AttachViews, const Vector2N& Size) override;
};