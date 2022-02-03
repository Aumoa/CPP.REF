// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXRenderPass.h"
#include "DirectXView.h"
#include "DirectXResource.h"

GENERATE_BODY(SDirectXRenderPass);

SDirectXRenderPass::SDirectXRenderPass(SDirectXDevice* Owner)
	: Super(Owner)
{
}

void SDirectXRenderPass::AddColorAttachment(const RHIRenderPassAttachment& AttachDesc)
{
	RenderTargets.emplace_back() =
	{
		.cpuDescriptor = {},
		.BeginningAccess = { .Type = (D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE)AttachDesc.LoadOp, .Clear = {} },
		.EndingAccess = { .Type = (D3D12_RENDER_PASS_ENDING_ACCESS_TYPE)(AttachDesc.StoreOp), .Resolve = {} },
	};

	TransitionBarriers.emplace_back() =
	{
		.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		.Transition =
		{
			.pResource = nullptr,
			.Subresource = 0,
			.StateBefore = (D3D12_RESOURCE_STATES)AttachDesc.BeforeState,
			.StateAfter = (D3D12_RESOURCE_STATES)AttachDesc.AfterState,
		}
	};
}

void SDirectXRenderPass::Apply(std::span<std::pair<IRHIView*, int32> const> AttachViews, const Vector2N& Size)
{
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> Handles;
	std::vector<ID3D12Resource*> pAttachResources;
	Handles.reserve(AttachViews.size());
	pAttachResources.reserve(AttachViews.size());

	for (auto& [View, IndexOf] : AttachViews)
	{
		if (auto* sView = Cast<SDirectXView>(View))
		{
			Handles.emplace_back(sView->GetCPUHandle(IndexOf));
			pAttachResources.emplace_back(Cast<SDirectXResource>(sView->GetResource(IndexOf))->pResource.Get());
		}
	}

	for (size_t i = 0; i < Handles.size(); ++i)
	{
		RenderTargets[i].cpuDescriptor = Handles[i];
		TransitionBarriers[i].Transition.pResource = pAttachResources[i];
	}
}