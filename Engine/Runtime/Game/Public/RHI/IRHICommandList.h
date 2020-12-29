// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHIRenderTargetView;
interface IRHIResource;
enum class RHIResourceStates;

interface IRHICommandList : virtual public Object
{
	virtual void BeginCommand() = 0;
	virtual void EndCommand() = 0;

	virtual void OMSetRenderTargets(size_t count, IRHIRenderTargetView* rtv[]) = 0;
	virtual void ClearRenderTargetView(IRHIRenderTargetView* rtv) = 0;
	virtual void ResourceTransition(IRHIResource* resource, RHIResourceStates beforeState, RHIResourceStates afterState, size_t subresourceIndex = 0) = 0;
	virtual void CopyResource(IRHIResource* target, IRHIResource* source) = 0;

	vs_property_get(bool, HasBegunCommand);
	virtual bool HasBegunCommand_get() const = 0;
};