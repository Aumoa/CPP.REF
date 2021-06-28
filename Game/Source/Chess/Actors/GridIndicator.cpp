// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GridIndicator.h"
#include "GameEngine.h"
#include "Components/StaticMeshComponent.h"
#include "Assets/Cube.h"

AGridIndicator::AGridIndicator() : Super()
{
	_StaticMeshComponent = CreateSubobject<StaticMeshComponent>();
	SetRootComponent(_StaticMeshComponent);

	_MeshObject = CreateSubobject<Cube>(L"CubeAsset", GameEngine::GetEngine());
	_StaticMeshComponent->SetStaticMesh(_MeshObject);
}