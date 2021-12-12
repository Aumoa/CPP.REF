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
	virtual std::shared_ptr<IRHIDeviceContext> GetImmediateContext() = 0;
	virtual std::shared_ptr<IRHIDeviceContext> CreateDeviceContext() = 0;
	virtual std::shared_ptr<IRHIDeviceContext2D> CreateDeviceContext2D() = 0;
	virtual std::shared_ptr<IRHITexture2D> CreateTexture2D(const RHITexture2DDesc& InDesc, const RHISubresourceData* InitialData) = 0;
	virtual std::shared_ptr<IRHIBuffer> CreateBuffer(const RHIBufferDesc& InDesc, const RHISubresourceData* InitialData) = 0;
	virtual std::shared_ptr<IRHIShader> CompileShader(SMaterial* Material) = 0;
	virtual std::shared_ptr<IRHIRenderTargetView> CreateRenderTargetView(int32 Count) = 0;
	virtual std::shared_ptr<IRHIDepthStencilView> CreateDepthStencilView(int32 Count) = 0;
	virtual std::shared_ptr<IRHIShaderResourceView> CreateShaderResourceView(int32 Count) = 0;
	virtual std::shared_ptr<IRHISolidColorBrush> CreateSolidColorBrush(const Color& InColor, float InOpacity = 1.0f) = 0;
	virtual std::shared_ptr<IRHIBitmap> CreateBitmapFromTexture2D(IRHITexture2D* InTexture) = 0;

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;
	virtual void FlushCommands() = 0;
};