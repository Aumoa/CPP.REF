// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Levels/StartupLevel.h"

#include "Engine.h"
#include "GameInstance.h"
#include "Framework/StaticMeshActor.h"
#include "Framework/DirectionalLight.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Assets/AssetManager.h"

GStartupLevel::GStartupLevel() : Super()
	, light(nullptr)
	
	, plane(nullptr)
	, geosphere(nullptr)
	, teapot(nullptr)
	, cylinder(nullptr)
	, cone(nullptr)
	, icosahedron(nullptr)

	, spectator(nullptr)
{

}

GStartupLevel::~GStartupLevel()
{

}

void GStartupLevel::LoadLevel()
{
	World* world = GetWorld();
	GameInstance* gameInstance = world->GetGameInstance();
	Engine* engine = gameInstance->GetEngine();
	AssetManager* assetMgr = engine->GetAssetManager();

	//light = SpawnActorPersistent<ADirectionalLight>();
	//light->SetActorTransform(Transform(Vector3(0, 100, 0), Vector3::One, Quaternion::LookTo(Vector3(-1, -1, 1), Vector3(0, 1, 0))));
	//light->LightComponent->LightColor = Color::White;

	plane = SpawnActorPersistent<AStaticMeshActor>();
	plane->StaticMesh->SetStaticMesh(assetMgr->LoadStaticMesh(L"Engine/StaticMesh/Box"));
	plane->StaticMesh->Scale = Vector3(10.0f, 0.1f, 10.0f);
	geosphere = SpawnActorPersistent<AStaticMeshActor>();
	geosphere->StaticMesh->SetStaticMesh(assetMgr->LoadStaticMesh(L"Engine/StaticMesh/GeoSphere"));
	geosphere->RootComponent->Location = Vector3(0, 0.55f, 0);
	teapot = SpawnActorPersistent<AStaticMeshActor>();
	teapot->StaticMesh->SetStaticMesh(assetMgr->LoadStaticMesh(L"Engine/StaticMesh/Teapot"));
	teapot->RootComponent->Location = Vector3(-1.0f, 0.55f, 0);
	cylinder = SpawnActorPersistent<AStaticMeshActor>();
	cylinder->StaticMesh->SetStaticMesh(assetMgr->LoadStaticMesh(L"Engine/StaticMesh/Cylinder"));
	cylinder->StaticMesh->Location = Vector3(1.0f, 0.55f, 0);
	cone = SpawnActorPersistent<AStaticMeshActor>();
	cone->StaticMesh->SetStaticMesh(assetMgr->LoadStaticMesh(L"Engine/StaticMesh/Cone"));
	cone->StaticMesh->Location = Vector3(0, 0.55f, -1.0f);
	icosahedron = SpawnActorPersistent<AStaticMeshActor>();
	icosahedron->StaticMesh->SetStaticMesh(assetMgr->LoadStaticMesh(L"Engine/StaticMesh/Icosahedron"));
	icosahedron->StaticMesh->Location = Vector3(0, 0.55f, 1.0f);
	icosahedron->StaticMesh->Scale = 0.5f;

	spectator = SpawnActorPersistent<ASpectatorPawn>();
}

APawn* GStartupLevel::GetPersistentActor() const
{
	return spectator;
}