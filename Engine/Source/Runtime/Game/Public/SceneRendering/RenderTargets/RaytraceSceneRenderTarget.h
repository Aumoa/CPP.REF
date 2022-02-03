// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneRenderTarget.h"

interface IRHITexture2D;

class GAME_API SRaytraceSceneRenderTarget : public SSceneRenderTarget
{
	GENERATED_BODY(SRaytraceSceneRenderTarget)

private:
	SPROPERTY(ColorBuffer)
	IRHITexture2D* ColorBuffer = nullptr;

public:
	SRaytraceSceneRenderTarget();

	using Super::Dispose;
	virtual void ResizeBuffers(const Vector2N& Size) override;

protected:
	virtual void Dispose(bool bDisposing) override;
};