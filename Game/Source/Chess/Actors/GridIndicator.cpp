// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GridIndicator.h"
#include "GameEngine.h"
#include "Components/StaticMeshComponent.h"
#include "Assets/Cube.h"
#include "Materials/MaterialInstance.h"
#include "Shaders/TransparentShader/TransparentShader.h"

AGridIndicator::AGridIndicator() : Super()
{
	GameEngine* engine = GameEngine::GetEngine();

	_StaticMeshComponent = CreateSubobject<StaticMeshComponent>();
	SetRootComponent(_StaticMeshComponent);

	_MeshObject = CreateSubobject<Cube>(L"CubeAsset", engine);
	_StaticMeshComponent->SetStaticMesh(_MeshObject);

	auto* mi = CreateSubobject<MaterialInstance>(engine->GetTransparentShader()->GetDefaultMaterial());
	mi->SetScalarParameterValueByName(L"Color", 1.0f);
	mi->SetScalarParameterValueByName(L"Alpha", 0.5f);
	_StaticMeshComponent->SetMaterial(0, mi);
}