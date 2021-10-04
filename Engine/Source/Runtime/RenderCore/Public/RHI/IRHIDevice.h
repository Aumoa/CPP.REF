// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIFactoryChild.h"
#include "RHIStructures.h"

interface IRHIDeviceContext;
interface IRHITexture2D;
interface IRHIBuffer;
interface IRHIShader;
interface IRHIRenderTargetView;
interface IRHIDepthStencilView;
interface IRHIShaderResourceView;
class SMaterial;
class SVertexFactory;

interface IRHIDevice : implements IRHIFactoryChild
{
	virtual IRHIDeviceContext* GetImmediateContext() = 0;
	virtual IRHITexture2D* CreateTexture2D(const RHITexture2DDesc& desc, const RHISubresourceData* initialData) = 0;
	virtual IRHIBuffer* CreateBuffer(const RHIBufferDesc& desc, const RHISubresourceData* initialData) = 0;
	virtual IRHIShader* CompileMaterial(SMaterial* material, SVertexFactory* vertexDeclaration) = 0;
	virtual IRHIRenderTargetView* CreateRenderTargetView(int32 count) = 0;
	virtual IRHIDepthStencilView* CreateDepthStencilView(int32 count) = 0;
	virtual IRHIShaderResourceView* CreateShaderResourceView(int32 count) = 0;
};