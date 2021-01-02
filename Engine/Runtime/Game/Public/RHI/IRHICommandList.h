// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHIRenderTargetView;
interface IRHIResource;
interface IRHIShader;
enum class ERHIResourceStates;
enum class ERHIPrimitiveTopology;
struct RHIViewport;
struct RHIMeshDrawCommand;

interface IRHICommandList : virtual public Object
{
	virtual void BeginCommand() = 0;
	virtual void EndCommand() = 0;

	virtual void SetRenderTargets(size_t count, IRHIRenderTargetView* rtv[]) = 0;
	virtual void ClearRenderTargetView(IRHIRenderTargetView* rtv) = 0;
	virtual void ResourceTransition(IRHIResource* resource, ERHIResourceStates beforeState, ERHIResourceStates afterState, size_t subresourceIndex = 0) = 0;
	virtual void CopyResource(IRHIResource* target, IRHIResource* source) = 0;
	virtual void SetShader(IRHIShader* shader) = 0;
	virtual void SetPrimitiveTopology(ERHIPrimitiveTopology primitiveTopology) = 0;
	virtual void SetScissorRects(const Rect& scissorRect) = 0;
	virtual void SetViewports(const RHIViewport& viewport) = 0;
	virtual void DrawMesh(const RHIMeshDrawCommand& command) = 0;

	vs_property_get(bool, HasBegunCommand);
	virtual bool HasBegunCommand_get() const = 0;
};