// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "SceneRenderer.h"

class RHIShaderLibrary;
class SceneVisibility;

class GAME_API DeferredSceneRenderer : public SceneRenderer
{
public:
	using Super = SceneRenderer;
	using This = DeferredSceneRenderer;

private:
	RHIShaderLibrary* shaderLibrary;

public:
	DeferredSceneRenderer(Scene* inScene);
	~DeferredSceneRenderer();

	virtual void RenderScene(ID3D12GraphicsCommandList4* immediateCommandList);

private:
	void SetShader(ID3D12GraphicsCommandList4* commandList);
	void RenderSceneInternal(ID3D12GraphicsCommandList4* commandList, SceneVisibility* inSceneVisibility);
};