// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceChild.h"
#include "Numerics/VectorInterface/Vector.h"
#include "ShaderParams/SlateShaderPaintGeometry.h"
#include "ShaderParams/SlateShaderRenderParams.h"

class NRHIViewport;
class NRHIRootSignature;
class NRHIDescriptorHeap;
class NRHISlateShader;
class NRHIGameShader;
struct NSlateShaderPaintGeometry;

class RHI_API NRHICommandSet : public NRHIDeviceChild
{
protected:
	NRHICommandSet() = default;

public:
	virtual ~NRHICommandSet() noexcept override = default;

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;

	virtual void BeginRender(const NRHIViewport& InViewport, bool bClear) = 0;
	virtual void EndRender(const NRHIViewport& InViewport) = 0;
	virtual void SetDescriptorHeaps(std::span<NRHIDescriptorHeap* const> Heaps) = 0;

	virtual void SetSlateShader(const NRHISlateShader& InShader) = 0;
	virtual void SetScreenResolutionInfo(const Vector2& InConstant) = 0;
	virtual void SetPaintGeometry(int64 VirtualAddress) = 0;
	virtual void SetRenderParams(int64 VirtualAddress) = 0;
	virtual void SetSlateInputTexture(int64 VirtualHandle) = 0;
	virtual void DrawSlateInstance() = 0;

	virtual void SetGameShader(const NRHIGameShader& InShader) = 0;
};