// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:PrimitiveComponent;

import :SceneComponent;

export class PrimitiveSceneProxy;

export class PrimitiveComponent : public SceneComponent
{
public:
	using Super = SceneComponent;

public:
	PrimitiveComponent();

	virtual PrimitiveSceneProxy* CreateSceneProxy() { return nullptr; }
};