// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHIRenderTargetView;
interface IRHIResource;
interface IRHIShader;
enum class RHIResourceStates;
enum class RHIPrimitiveTopology;
struct RHIViewport;

interface GAME_API IRHICommandList : virtual public Object
{
	virtual void BeginCommand() = 0;
	virtual void EndCommand() = 0;

	virtual void SetRenderTargets(size_t count, IRHIRenderTargetView* rtv[]) = 0;
	virtual void ClearRenderTargetView(IRHIRenderTargetView* rtv) = 0;
	virtual void ResourceTransition(IRHIResource* resource, RHIResourceStates beforeState, RHIResourceStates afterState, size_t subresourceIndex = 0) = 0;
	virtual void CopyResource(IRHIResource* target, IRHIResource* source) = 0;
	virtual void SetShader(IRHIShader* shader) = 0;
	virtual void DrawInstanced(uint32 vertexCountPerInstance, uint32 instanceCount, int32 baseVertexLocation, uint32 startInstanceLocation) = 0;
	virtual void DrawIndexedInstanced(uint32 indexCountPerInstance, uint32 instanceCount, uint32 startIndexLocation, int32 baseVertexLocation, uint32 startInstanceLocation) = 0;
	virtual void SetPrimitiveTopology(RHIPrimitiveTopology primitiveTopology) = 0;
	virtual void SetScissorRects(const Rect& scissorRect) = 0;
	virtual void SetViewports(const RHIViewport& viewport) = 0;

	vs_property_get(bool, HasBegunCommand);
	virtual bool HasBegunCommand_get() const = 0;
};