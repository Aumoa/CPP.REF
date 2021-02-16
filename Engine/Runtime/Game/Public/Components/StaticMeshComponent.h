// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "MeshComponent.h"

class StaticMesh;
class MaterialInterface;

class GAME_API StaticMeshComponent : public MeshComponent
{
public:
	using Super = MeshComponent;
	using This = StaticMeshComponent;

private:
	TWeakPtr<StaticMesh> staticMesh;
	TRefPtr<MaterialInterface> overrideMaterial;

public:
	StaticMeshComponent();
	~StaticMeshComponent() override;

	TRefPtr<PrimitiveSceneProxy> CreateSceneProxy() override;

	void SetStaticMesh(StaticMesh* inMesh);
	StaticMesh* GetStaticMesh() const;
	void SetMaterial(MaterialInterface* inMaterial);
	MaterialInterface* GetMaterial() const;
};