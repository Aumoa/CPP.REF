// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "SceneRenderer.h"

interface IRHIShader;

class GAME_API DeferredSceneRenderer : public SceneRenderer
{
public:
	using Super = SceneRenderer;
	using This = DeferredSceneRenderer;

private:
	static bool bShaderCompiled;
	static IRHIShader* pickShader;

public:
	DeferredSceneRenderer(Scene* inScene);
	~DeferredSceneRenderer();

	virtual void RenderScene(IRHICommandList* immediateCommandList);

private:
	static void ShaderInitialize();

	void SetShader(IRHICommandList* commandList);
	void RenderSceneInternal(IRHICommandList* commandList, const std::vector<bool>& primitiveVisibility);
};