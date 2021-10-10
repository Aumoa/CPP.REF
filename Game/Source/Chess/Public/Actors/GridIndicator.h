// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

class SStaticMesh;
class SStaticMeshComponent;
class SMaterialInstance;

class CHESS_API AGridIndicator : public AActor
{
	GENERATED_BODY(AGridIndicator)

private:
	SStaticMesh* _MeshObject = nullptr;
	SStaticMeshComponent* _StaticMeshComponent = nullptr;
	SMaterialInstance* _materialInterface = nullptr;

public:
	AGridIndicator();

	void SetIndicatorColor(const Color& value);
};