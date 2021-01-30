// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Transform.h"
#include "ComponentMobility.h"

class MeshBatch;
class PrimitiveComponent;

class GAME_API PrimitiveSceneProxy : public Object
{
public:
	using Super = Object;
	using This = PrimitiveSceneProxy;

private:
	PrimitiveComponent* myPrimitiveComponent;
	Transform transform;

public:
	PrimitiveSceneProxy(PrimitiveComponent* inPrimitiveComponent);
	~PrimitiveSceneProxy();

	void UpdateMovable();

	virtual MeshBatch* GetMeshBatch() const;
	virtual Transform GetPrimitiveTransform() const;

	const EComponentMobility Mobility;
};