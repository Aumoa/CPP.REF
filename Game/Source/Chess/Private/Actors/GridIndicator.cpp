// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Actors/GridIndicator.h"
#include "GameEngine.h"
#include "Components/StaticMeshComponent.h"
#include "Assets/Cube.h"
#include "Materials/MaterialInstance.h"
#include "Shaders/TransparentShader/TransparentShader.h"

AGridIndicator::AGridIndicator() : Super()
{
	_StaticMeshComponent = NewObject<SStaticMeshComponent>();
	SetRootComponent(_StaticMeshComponent);
	_StaticMeshComponent->SetHiddenInGame(true);

	_MeshObject = NewObject<Cube>(L"CubeAsset", GEngine);
	_StaticMeshComponent->SetStaticMesh(_MeshObject);

	//_materialInterface = NewObject<SMaterialInstance>(GEngine->GetTransparentShader()->GetDefaultMaterial());
	//_materialInterface->SetScalarParameterValueByName(L"Alpha", 0.5f);
	//_StaticMeshComponent->SetMaterial(0, _materialInterface);
}

void AGridIndicator::SetIndicatorColor(const Color& value)
{
	//_materialInterface->SetVector3ParameterValueByName(L"Color", value);
}