// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "MeshComponent.h"
#include "StaticMeshComponent.gen.h"

SCLASS()
class GAME_API StaticMeshComponent : public MeshComponent
{
	GENERATED_BODY()

protected:
	virtual PrimitiveSceneProxy* CreateSceneProxy() override;
};