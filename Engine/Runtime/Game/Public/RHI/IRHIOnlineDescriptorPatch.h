// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHIShaderResourceView;
interface IRHIUnorderedAccessView;

interface GAME_API IRHIOnlineDescriptorPatch : virtual public Object
{
	virtual void ReallocAndStart(size_t num) = 0;
	virtual size_t Patch(IRHIShaderResourceView* srv) = 0;
	virtual size_t Patch(IRHIUnorderedAccessView* uav) = 0;
};