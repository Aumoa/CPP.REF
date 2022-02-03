// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "MeshComponent.h"
#include "SceneRendering/MeshBatch.h"

class SStaticMesh;
class SMaterial;

class GAME_API SStaticMeshComponent : public SMeshComponent
{
	GENERATED_BODY(SStaticMeshComponent)

private:
	SStaticMesh* _StaticMesh = nullptr;
	MeshBatch _batch;
	std::vector<SMaterial*> _materials;

public:
	SStaticMeshComponent();

	virtual PrimitiveSceneProxy* CreateSceneProxy() override;

	virtual void SetStaticMesh(SStaticMesh* inNewMesh);
	SStaticMesh* GetStaticMesh() { return _StaticMesh; }

	void SetMaterial(int32 index, SMaterial* material);
	SMaterial* GetMaterial(int32 index);
	int32 GetMaterialCount();
};