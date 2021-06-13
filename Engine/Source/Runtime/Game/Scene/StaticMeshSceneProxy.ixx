// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:StaticMeshSceneProxy;

import :PrimitiveSceneProxy;

export class StaticMeshComponent;

export class StaticMeshSceneProxy : public PrimitiveSceneProxy
{
public:
	using Super = PrimitiveSceneProxy;

public:
	StaticMeshSceneProxy(StaticMeshComponent* inComponent);
};