// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIGlobal.h"
#include "D3D12RHI/D3D12Graphics.h"
#include "D3D12RHI/D3D12CommandQueue.h"

struct ND3D12Global : public NRHIGlobal
{
	static ND3D12Graphics& GetDynamicRHI() { return static_cast<ND3D12Graphics&>(NRHIGlobal::GetDynamicRHI()); }
	static ND3D12CommandQueue& GetPrimaryCommandQueue() { return static_cast<ND3D12CommandQueue&>(NRHIGlobal::GetPrimaryCommandQueue()); }
};