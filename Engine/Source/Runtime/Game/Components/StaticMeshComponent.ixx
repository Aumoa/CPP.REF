// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:StaticMeshComponent;

import :MeshComponent;

export class StaticMesh;

export class StaticMeshComponent : public MeshComponent
{
public:
	using Super = MeshComponent;

private:
	StaticMesh* _StaticMesh = nullptr;

public:
	StaticMeshComponent();

	virtual PrimitiveSceneProxy* CreateSceneProxy() override;

	virtual void SetStaticMesh(StaticMesh* inNewMesh);
	StaticMesh* GetStaticMesh() const { return _StaticMesh; }
};