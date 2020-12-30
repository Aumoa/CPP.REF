// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "PrimitiveSceneProxy.h"

class StaticMeshComponent;

class GAME_API StaticMeshSceneProxy : public PrimitiveSceneProxy
{
public:
	using Super = PrimitiveSceneProxy;
	using This = StaticMeshSceneProxy;

public:
	StaticMeshSceneProxy(StaticMeshComponent* inMeshComponent);
	~StaticMeshSceneProxy();
};