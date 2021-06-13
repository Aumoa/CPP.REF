// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:SceneRenderer;

import SC.Runtime.Core;
import SC.Runtime.RenderCore;
import SC.Runtime.Game.Shaders;

export class Scene;
export class PrimitiveSceneProxy;

export class SceneRenderer : virtual public Object
{
public:
	using Super = Object;

private:
	Scene* _scene = nullptr;
	ColorShader* _shader = nullptr;

public:
	SceneRenderer(Scene* scene, ColorShader* shader);

	void RenderScene(RHIDeviceContext* dc);

private:
	void RenderPrimitive(RHIDeviceContext* dc, PrimitiveSceneProxy* proxy);
};