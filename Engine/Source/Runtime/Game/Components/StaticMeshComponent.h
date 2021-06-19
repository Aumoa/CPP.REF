// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "MeshComponent.h"

class StaticMesh;

class StaticMeshComponent : public MeshComponent
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