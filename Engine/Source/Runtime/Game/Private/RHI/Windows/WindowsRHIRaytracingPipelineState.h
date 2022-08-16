// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIRaytracingPipelineState.h"

#if PLATFORM_WINDOWS

#include "RHI/RHIStructures.h"
#include "RHI/Windows/WindowsRHICommon.h"

class WindowsRHIDevice;
class RHIShaderBytecode;

class WindowsRHIRaytracingPipelineState : public RHIRaytracingPipelineState
{
	friend WindowsRHIDevice;

private:
	ComPtr<ID3D12StateObject> _pipeline;
	ComPtr<ID3D12StateObjectProperties> _properties;

private:
	WindowsRHIRaytracingPipelineState(std::shared_ptr<WindowsRHIDevice> device, const RHIShaderLibraryExport& shaderExport);

public:
	virtual ~WindowsRHIRaytracingPipelineState() noexcept override;

public:
	DECLGET(StateObject, _pipeline);
	DECLGET(Properties, _properties);
};

#endif