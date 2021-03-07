// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "MeshComponent.h"

class StaticMesh;
class MaterialInterface;

class GAME_API GStaticMeshComponent : public GMeshComponent
{
public:
	using Super = GMeshComponent;
	using This = GStaticMeshComponent;

private:
	TWeakPtr<StaticMesh> staticMesh;
	TRefPtr<MaterialInterface> overrideMaterial;

public:
	GStaticMeshComponent();
	~GStaticMeshComponent() override;

	TRefPtr<PrimitiveSceneProxy> CreateSceneProxy() override;

	void SetStaticMesh(StaticMesh* inMesh);
	StaticMesh* GetStaticMesh() const;
	void SetMaterial(MaterialInterface* inMaterial);
	MaterialInterface* GetMaterial() const;
};