// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class GAME_API SSceneRenderTarget : implements SObject, implements IDisposable
{
	GENERATED_BODY(SSceneRenderTarget)

public:
	SSceneRenderTarget();

	virtual void Dispose() override;
	virtual void ResizeBuffers(const Vector2N& Size) = 0;

protected:
	virtual void Dispose(bool bDisposing) = 0;
};