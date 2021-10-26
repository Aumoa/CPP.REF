// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneRenderer.h"

class SSlateShader;
class SlateWindowElementList;

class GAME_API SlateRenderer : public SceneRenderer
{
	GENERATED_BODY(SlateRenderer)

public:
	SSlateShader* const Shader;
	const SlateWindowElementList& Elements;

public:
	SlateRenderer(SceneRenderTarget& InRenderTarget, SSlateShader* SlateShader, const SlateWindowElementList& Elements);

protected:
	virtual void OnPopulateCommandLists(IRHIDeviceContext* Context) override;
};