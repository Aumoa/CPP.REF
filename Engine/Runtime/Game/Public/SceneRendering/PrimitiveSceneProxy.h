// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class MeshBatch;
class PrimitiveComponent;

class GAME_API PrimitiveSceneProxy : public Object
{
public:
	using Super = Object;
	using This = PrimitiveSceneProxy;

private:
	PrimitiveComponent* myPrimitiveComponent;

public:
	PrimitiveSceneProxy(PrimitiveComponent* inPrimitiveComponent);
	~PrimitiveSceneProxy();

	virtual MeshBatch* GetMeshBatch() const;
};