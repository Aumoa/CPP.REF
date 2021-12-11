// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneRenderTargetInterface.h"

interface IRHITexture2D;
interface IRHIBitmap;

class RENDERCORE_API SColorRenderTarget : public SSceneRenderTargetInterface
{
	GENERATED_BODY(SColorRenderTarget)

private:
	SPROPERTY(Device)
	IRHIDevice* Device = nullptr;
	SPROPERTY(RenderTarget)
	IRHITexture2D* RenderTarget = nullptr;
	SPROPERTY(RenderBitmap)
	IRHIBitmap* RenderBitmap = nullptr;

public:
	SColorRenderTarget(IRHIDevice* InDevice, const Vector2N& InitialViewportSize);

	virtual IRHITexture2D* GetRenderTexture() override;
	IRHIBitmap* GetRenderBitmap();

	void SetViewportSize(const Vector2N& InViewportSize);
};