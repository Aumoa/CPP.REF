// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHICommandList.h"

#include "VulkanMinimal.h"

class VulkanCommandList : virtual public Object, virtual public IRHICommandList
{
public:
	using Super = Object;
	using This = VulkanCommandList;

private:

public:
	VulkanCommandList();
	~VulkanCommandList() override;

	virtual void BeginCommand();
	virtual void EndCommand();

	virtual void OMSetRenderTargets(size_t count, IRHIRenderTargetView* rtv[]);
	virtual void ClearRenderTargetView(IRHIRenderTargetView* rtv);
	virtual void ResourceTransition(IRHIResource* resource, ERHIResourceStates beforeState, ERHIResourceStates afterState, size_t subresourceIndex = 0);
	virtual void CopyResource(IRHIResource* target, IRHIResource* source);

	virtual bool HasBegunCommand_get() const;
};