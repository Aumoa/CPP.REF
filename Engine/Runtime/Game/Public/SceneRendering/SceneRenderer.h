// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHIResource;
interface IRHIDeferredCommandList;
interface IRHIDeviceBundle;
interface IRHIRenderTargetView;
interface IRHICommandList;
interface IRHIShader;

class PrimitiveSceneProxy;
class Scene;
class SceneVisibility;

struct MinimalViewInfo;

class GAME_API SceneRenderer : virtual public Object
{
public:
	using Super = Object;
	using This = SceneRenderer;

private:
	static bool bShaderCompiled;
	static IRHIShader* pickShader;

	Scene* renderScene;
	std::vector<SceneVisibility> visibilities;

public:
	SceneRenderer(Scene* scene);
	~SceneRenderer() override;

	virtual void CalcVisibility(MinimalViewInfo& inView);
	virtual void RenderScene(IRHICommandList* immediateCommandList);

	void CalcLocalPlayerVisibility();

private:
	static void ShaderInitialize();

	void SetShader(IRHICommandList* commandList);
	void RenderSceneInternal(IRHICommandList* commandList, const std::vector<bool>& primitiveVisibility);
};