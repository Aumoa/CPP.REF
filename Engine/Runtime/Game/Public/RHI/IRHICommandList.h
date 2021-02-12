
// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHIRenderTargetView;
interface IRHIResource;
interface IRHIShader;
interface IRHIDepthStencilView;
interface IRHIShaderResourceView;
enum class ERHIResourceStates;
enum class ERHIPrimitiveTopology;
struct RHIViewport;
struct RHIMeshDrawCommand;

interface IRHICommandList : virtual public Object
{
	virtual void BeginCommand() = 0;
	virtual void EndCommand() = 0;

	virtual void SetRenderTargets(size_t count, IRHIRenderTargetView* rtv[], IRHIDepthStencilView* dsv) = 0;
	virtual void ClearRenderTargetView(IRHIRenderTargetView* rtv) = 0;
	virtual void ResourceTransition(IRHIResource* resource, ERHIResourceStates beforeState, ERHIResourceStates afterState, size_t subresourceIndex = 0) = 0;
	virtual void CopyResource(IRHIResource* target, IRHIResource* source) = 0;
	virtual void SetShader(IRHIShader* shader) = 0;
	virtual void SetPrimitiveTopology(ERHIPrimitiveTopology primitiveTopology) = 0;
	virtual void SetScissorRects(const Rect& scissorRect) = 0;
	virtual void SetViewports(const RHIViewport& viewport) = 0;
	virtual void DrawMesh(const RHIMeshDrawCommand& command) = 0;
	virtual void SetGraphicsRootConstantBufferView(uint32 inParamIndex, uint64 inVirtualAddress) = 0;
	virtual void ClearDepthStencilView(IRHIDepthStencilView* dsv, std::optional<float> depth, std::optional<uint8> stencil) = 0;
	virtual void SetGraphicsRootShaderResourceView(uint32 inRootParameterIndex, IRHIShaderResourceView* inSRV) = 0;
	virtual void CopyBufferRegion(IRHIResource* inDstBuffer, uint64 inDstLocation, IRHIResource* inSrcBuffer, uint64 inSrcLocation, uint64 inNumBytes) = 0;
	virtual void SetGraphicsRoot32BitConstants(uint32 inParamIndex, const uint32* inBytes, size_t inNum32Bits, size_t location = 0) = 0;
	virtual void SetGraphicsRootShaderResource(uint32 inParamIndex, uint64 inVirtualAddress) = 0;

	vs_property_get(bool, HasBegunCommand);
	virtual bool HasBegunCommand_get() const = 0;
};