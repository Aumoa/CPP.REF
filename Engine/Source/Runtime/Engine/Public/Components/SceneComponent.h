// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Numerics/TransformInterface/Transform.h"
#include "SceneComponent.generated.h"

ACLASS()
class ENGINE_API ASceneComponent : public AActorComponent
{
	GENERATED_BODY()

private:
	Transform WorldTransform;

public:
	ASceneComponent();
};