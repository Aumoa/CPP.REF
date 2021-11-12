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
interface IRHIDeviceContext2D;
interface IRHISolidColorBrush;
interface IRHIBitmap;
class SMaterial;

interface IRHIDevice : implements IRHIFactoryChild
{
	virtual IRHIDeviceContext* GetImmediateContext() = 0;
	virtual IRHIDeviceContext* CreateDeviceContext() = 0;
	virtual IRHIDeviceContext2D* CreateDeviceContext2D() = 0;
	virtual IRHITexture2D* CreateTexture2D(const RHITexture2DDesc& InDesc, const RHISubresourceData* InitialData) = 0;
	virtual IRHIBuffer* CreateBuffer(const RHIBufferDesc& InDesc, const RHISubresourceData* InitialData) = 0;
	virtual IRHIShader* CompileShader(SMaterial* Material) = 0;
	virtual IRHIRenderTargetView* CreateRenderTargetView(int32 Count) = 0;
	virtual IRHIDepthStencilView* CreateDepthStencilView(int32 Count) = 0;
	virtual IRHIShaderResourceView* CreateShaderResourceView(int32 Count) = 0;
	virtual IRHISolidColorBrush* CreateSolidColorBrush(const Color& InColor, float InOpacity = 1.0f) = 0;
	virtual IRHIBitmap* CreateBitmapFromTexture2D(IRHITexture2D* InTexture) = 0;

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;
	virtual void FlushCommands() = 0;
};