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
interface IRHITextLayout;
interface IRHITextFormat;
class SMaterial;
class SVertexFactory;

interface IRHIDevice : implements IRHIFactoryChild
{
	virtual IRHIDeviceContext* GetImmediateContext() = 0;
	virtual IRHIDeviceContext* CreateDeviceContext() = 0;
	virtual IRHITexture2D* CreateTexture2D(const RHITexture2DDesc& desc, const RHISubresourceData* initialData) = 0;
	virtual IRHIBuffer* CreateBuffer(const RHIBufferDesc& desc, const RHISubresourceData* initialData) = 0;
	virtual IRHIShader* CompileMaterial(SMaterial* material, SVertexFactory* vertexDeclaration) = 0;
	virtual IRHIRenderTargetView* CreateRenderTargetView(int32 count) = 0;
	virtual IRHIDepthStencilView* CreateDepthStencilView(int32 count) = 0;
	virtual IRHIShaderResourceView* CreateShaderResourceView(int32 count) = 0;
	virtual IRHITextLayout* CreateTextLayout(IRHITextFormat* format, std::wstring_view text, const Vector2& layout) = 0;
	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;
};