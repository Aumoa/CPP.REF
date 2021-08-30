// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "MeshComponent.h"

class StaticMesh;

class GAME_API StaticMeshComponent : public MeshComponent
{
	CLASS_BODY(StaticMeshComponent)

private:
	StaticMesh* _StaticMesh = nullptr;
	MeshBatch _batch;
	std::vector<Material*> _materials;

public:
	StaticMeshComponent();

	virtual PrimitiveSceneProxy* CreateSceneProxy() override;

	virtual void SetStaticMesh(StaticMesh* inNewMesh);
	StaticMesh* GetStaticMesh() const { return _StaticMesh; }

	void SetMaterial(int32 index, Material* material);
	Material* GetMaterial(int32 index) const;
	int32 GetMaterialCount() const;
};