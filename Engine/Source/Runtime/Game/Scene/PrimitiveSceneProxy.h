// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include "GameStructures.h"

class PrimitiveComponent;

class PrimitiveSceneProxy : virtual public Object
{
public:
	using Super = Object;

private:
	PrimitiveComponent* _MyComponent = nullptr;

public:
	PrimitiveSceneProxy(PrimitiveComponent* inComponent);

	PrimitiveComponent* GetComponent() const { return _MyComponent; }
	void UpdateTransform_GameThread(const Transform& value);

	std::vector<MeshBatch> MeshBatches;
	int64 PrimitiveId = -1;
	Transform ComponentTransform;
};