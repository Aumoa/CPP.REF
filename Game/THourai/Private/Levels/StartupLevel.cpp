// Copyright 2020 Aumoa.lib. All right reserved.

#include "Levels/StartupLevel.h"

#include "Engine.h"
#include "Pawn/MyCharacter.h"
#include "Framework/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Assets/AssetManager.h"

StartupLevel::StartupLevel() : Super()
	, plane(nullptr)
	, geosphere(nullptr)
	, teapot(nullptr)
	, cylinder(nullptr)
	, cone(nullptr)
	, icosahedron(nullptr)

	, spectator(nullptr)
{

}

StartupLevel::~StartupLevel()
{

}

void StartupLevel::LoadLevel()
{
	Engine* engine = Engine::GetInstance();
	AssetManager* assetMgr = engine->GetAssetManager();

	plane = SpawnActorPersistent<AStaticMeshActor>();
	plane->StaticMesh->SetStaticMesh(assetMgr->LoadStaticMesh(L"Engine/StaticMesh/Box"));
	plane->StaticMesh->Scale = Vector3(10.0f, 10.0f, 0.1f);
	geosphere = SpawnActorPersistent<AStaticMeshActor>();
	geosphere->StaticMesh->SetStaticMesh(assetMgr->LoadStaticMesh(L"Engine/StaticMesh/GeoSphere"));
	geosphere->RootComponent->Location = Vector3(0, 0, 0.55f);
	teapot = SpawnActorPersistent<AStaticMeshActor>();
	teapot->StaticMesh->SetStaticMesh(assetMgr->LoadStaticMesh(L"Engine/StaticMesh/Teapot"));
	teapot->RootComponent->Location = Vector3(-1.0f, 0, 0.55f);
	cylinder = SpawnActorPersistent<AStaticMeshActor>();
	cylinder->StaticMesh->SetStaticMesh(assetMgr->LoadStaticMesh(L"Engine/StaticMesh/Cylinder"));
	cylinder->StaticMesh->Location = Vector3(1.0f, 0, 0.55f);
	cone = SpawnActorPersistent<AStaticMeshActor>();
	cone->StaticMesh->SetStaticMesh(assetMgr->LoadStaticMesh(L"Engine/StaticMesh/Cone"));
	cone->StaticMesh->Location = Vector3(0, -1.0f, 0.55f);
	icosahedron = SpawnActorPersistent<AStaticMeshActor>();
	icosahedron->StaticMesh->SetStaticMesh(assetMgr->LoadStaticMesh(L"Engine/StaticMesh/Icosahedron"));
	icosahedron->StaticMesh->Location = Vector3(0, 1.0f, 0.55f);
	icosahedron->StaticMesh->Scale = 0.5f;

	spectator = SpawnActorPersistent<ASpectatorPawn>();
}

APawn* StartupLevel::GetPersistentActor() const
{
	return spectator;
}