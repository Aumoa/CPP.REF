// Copyright 2020 Aumoa.lib. All right reserved.

#include "Levels/StartupLevel.h"

#include "Engine.h"
#include "Pawn/MyCharacter.h"
#include "Framework/StaticMeshActor.h"
#include "Framework/DirectionalLight.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Assets/AssetManager.h"

StartupLevel::StartupLevel() : Super()
	, light(nullptr)
	, light2(nullptr)
	
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

	light = SpawnActorPersistent<ADirectionalLight>();
	light->SetActorTransform(Transform(Vector3(0, 100, 0), Vector3::One, Quaternion::LookTo(Vector3(-1, -1, 1), Vector3(0, 1, 0))));
	light->LightComponent->LightColor = Color::Red;

	light2 = SpawnActorPersistent<ADirectionalLight>();
	light2->SetActorTransform(Transform(Vector3(0, 100, 0), Vector3::One, Quaternion::LookTo(Vector3(-1, -1, -1), Vector3(0, 1, 0))));
	light2->LightComponent->LightColor = Color::Green;

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

	for (size_t i = 0; i < 2500; ++i)
	{
		int32 x = (int32)i % 50;
		int32 y = (int32)i / 50;
		sphere_2500[i] = SpawnActorPersistent<AStaticMeshActor>();
		sphere_2500[i]->StaticMesh->SetStaticMesh(assetMgr->LoadStaticMesh(L"Engine/StaticMesh/GeoSphere"));
		sphere_2500[i]->RootComponent->Location = Vector3((float)(-25 + x), 0.55f, (float)(-25 + y));
	}

	spectator = SpawnActorPersistent<ASpectatorPawn>();
}

APawn* StartupLevel::GetPersistentActor() const
{
	return spectator;
}