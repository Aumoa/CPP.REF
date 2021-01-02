// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "MeshComponent.h"

class StaticMesh;

class GAME_API StaticMeshComponent : public MeshComponent
{
public:
	using Super = MeshComponent;
	using This = StaticMeshComponent;

private:
	TWeakPtr<StaticMesh> staticMesh;

public:
	StaticMeshComponent();
	~StaticMeshComponent() override;

	void SetStaticMesh(StaticMesh* inMesh);
	StaticMesh* GetStaticMesh() const;

	TRefPtr<PrimitiveSceneProxy> CreateSceneProxy() override;
};