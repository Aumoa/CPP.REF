// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericImage.h"

class NGenericWindow;
class NRHICommandQueue;
class NRHIViewport;
class NRHITexture2D;
class NRHICommandSet;
class NRHIRootSignature;
class NRHIGraphicsPipelineState;
class NRHIConstantBuffer;
class NRHIDescriptorHeap;
class NRHIShaderResourceView;
class NRHITextFormat;
class NRHITextLayout;

class RHI_API NRHIGraphics
{
protected:
	NRHIGraphics();

public:
	virtual ~NRHIGraphics() noexcept;

	virtual void Init() = 0;
	virtual std::shared_ptr<NRHICommandQueue> GetPrimaryQueue() = 0;
	virtual std::shared_ptr<NRHICommandQueue> CreateCommandQueue() = 0;
	virtual std::shared_ptr<NRHIViewport> CreateViewport(NRHICommandQueue& InCommandQueue, NGenericWindow& InWindow) = 0;
	virtual Task<std::shared_ptr<NRHITexture2D>> CreateTexture2DAsync(std::shared_ptr<NGenericImage> ImageSource) = 0;
	virtual std::shared_ptr<NRHICommandSet> CreateCommandSet() = 0;
	virtual std::shared_ptr<NRHIRootSignature> CreateRootSignature() = 0;
	virtual std::shared_ptr<NRHIGraphicsPipelineState> CreateGraphicsPipelineState(NRHIRootSignature& InRS) = 0;
	virtual std::shared_ptr<NRHIConstantBuffer> CreateConstantBuffer() = 0;
	virtual std::shared_ptr<NRHIDescriptorHeap> CreateDescriptorHeap() = 0;
	virtual std::shared_ptr<NRHIShaderResourceView> CreateShaderResourceView(size_t InNumViews) = 0;
	virtual std::shared_ptr<NRHITextFormat> CreateTextFormat(String FontFamilyName, float FontSize, bool bBold, bool bItalic) = 0;
	virtual std::shared_ptr<NRHITextLayout> CreateTextLayout(std::shared_ptr<NRHITextFormat> InTextFormat, String InText) = 0;

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;
	virtual void SyncFrame() = 0;

public:
	static std::unique_ptr<NRHIGraphics> GenerateGraphics(String InRHIModuleName);
};