// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Transform.h"
#include "ComponentMobility.h"

class MeshBatch;
class GPrimitiveComponent;

class GAME_API PrimitiveSceneProxy : public Object
{
public:
	using Super = Object;
	using This = PrimitiveSceneProxy;

private:
	TWeakPtr<GPrimitiveComponent> myPrimitiveComponent;
	Transform transform;

public:
	PrimitiveSceneProxy(GPrimitiveComponent* inPrimitiveComponent);
	~PrimitiveSceneProxy();

	virtual void Update();
	virtual void UpdateTransform();

	virtual MeshBatch* GetMeshBatch() const;
	virtual Transform GetPrimitiveTransform() const;
	virtual const AxisAlignedCube* GetPrimitiveBoundingBox() const;
	virtual uint64 GetRaytracingAccelerationStructurePtr() const;

	const EComponentMobility Mobility;
};