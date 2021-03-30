// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "PrimitiveSceneProxy.h"

class GStaticMeshComponent;
class StaticMesh;
class MaterialInterface;

class GAME_API StaticMeshSceneProxy : public PrimitiveSceneProxy
{
public:
	using Super = PrimitiveSceneProxy;
	using This = StaticMeshSceneProxy;

private:
	TWeakPtr<GStaticMeshComponent> meshComponent;
	StaticMesh* staticMesh;

public:
	StaticMeshSceneProxy(GStaticMeshComponent* inMeshComponent);
	~StaticMeshSceneProxy();

	void Update() override;
	void UpdateTransform() override;
};