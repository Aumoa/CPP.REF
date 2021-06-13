// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:StaticMeshComponent;

import :MeshComponent;

export class StaticMeshComponent : public MeshComponent
{
public:
	using Super = MeshComponent;

public:
	StaticMeshComponent();

	virtual PrimitiveSceneProxy* CreateSceneProxy() override;
};