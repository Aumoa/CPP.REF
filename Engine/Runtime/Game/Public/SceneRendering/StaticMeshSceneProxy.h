// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "PrimitiveSceneProxy.h"

class StaticMeshComponent;
class StaticMesh;

class GAME_API StaticMeshSceneProxy : public PrimitiveSceneProxy
{
public:
	using Super = PrimitiveSceneProxy;
	using This = StaticMeshSceneProxy;

private:
	StaticMesh* staticMesh;
	AxisAlignedCube baseBoundingBox;
	AxisAlignedCube transformedBoundingBox;

public:
	StaticMeshSceneProxy(StaticMeshComponent* inMeshComponent);
	~StaticMeshSceneProxy();

	void UpdateMovable() override;

	MeshBatch* GetMeshBatch() const override;
	const AxisAlignedCube* GetPrimitiveBoundingBox() const override;
};