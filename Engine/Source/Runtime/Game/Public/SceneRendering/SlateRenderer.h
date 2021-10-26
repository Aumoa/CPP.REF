// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneRenderer.h"
#include "Shaders/SlateShader/SlateShader.h"

class SlateWindowElementList;

class GAME_API SlateRenderer : public SceneRenderer
{
	GENERATED_BODY(SlateRenderer)

public:
	SSlateShader* const Shader;
	const SSlateShader::InitElementContext* InitContext;

public:
	SlateRenderer(SceneRenderTarget& InRenderTarget, SSlateShader* SlateShader, const SSlateShader::InitElementContext* InitContext);

protected:
	virtual void OnPopulateCommandLists(IRHIDeviceContext* Context) override;
};