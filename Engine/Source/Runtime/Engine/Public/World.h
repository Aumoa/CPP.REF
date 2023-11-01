// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "World.generated.h"

class AActor;
class NSceneView;

ACLASS()
class ENGINE_API AWorld : public AObject
{
	GENERATED_BODY()

private:
	std::vector<RefPtr<AActor>> PersistentActors;
	std::shared_ptr<NSceneView> SceneView;

public:
	AWorld();
	virtual ~AWorld() noexcept override;

	void InitializeScene();
	void DestroyScene();

	AActor* SpawnPersistentActor(AType* InActorType);
	AWorld* GetWorld() { return this; }
};