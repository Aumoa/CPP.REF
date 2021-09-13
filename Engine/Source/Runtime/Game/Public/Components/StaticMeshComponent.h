// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "MeshComponent.h"

class SStaticMesh;

class GAME_API SStaticMeshComponent : public SMeshComponent
{
	GENERATED_BODY(SStaticMeshComponent)

private:
	SStaticMesh* _StaticMesh = nullptr;
	MeshBatch _batch;
	std::vector<SMaterial*> _materials;

public:
	SStaticMeshComponent();

	virtual SPrimitiveSceneProxy* CreateSceneProxy() override;

	virtual void SetStaticMesh(SStaticMesh* inNewMesh);
	SStaticMesh* GetStaticMesh() const { return _StaticMesh; }

	void SetMaterial(int32 index, SMaterial* material);
	SMaterial* GetMaterial(int32 index) const;
	int32 GetMaterialCount() const;
};