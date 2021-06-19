// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class Scene;
class RHIShader;
class RHIDeviceContext;
class SceneVisibility;
class PrimitiveSceneProxy;

class SceneRenderer : virtual public Object
{
public:
	using Super = Object;

private:
	Scene* _scene = nullptr;
	RHIShader* _shader = nullptr;

public:
	SceneRenderer(Scene* scene, RHIShader* shader);

	void RenderScene(RHIDeviceContext* dc);

private:
	void RenderWithSceneVisibility(RHIDeviceContext* dc, SceneVisibility* view);
	void RenderPrimitive(RHIDeviceContext* dc, PrimitiveSceneProxy* proxy);
};