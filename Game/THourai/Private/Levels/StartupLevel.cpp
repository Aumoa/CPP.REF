// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Levels/StartupLevel.h"

#include "Engine.h"
#include "GameInstance.h"
#include "Framework/StaticMeshActor.h"
#include "Framework/DirectionalLight.h"
#include "Framework/PointLight.h"
#include "Framework/SpotLight.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Assets/AssetManager.h"
#include "Assets/AssetImporter.h"
#include "Actor/RotateLight.h"
#include "SceneRendering/StaticMesh.h"

GStartupLevel::GStartupLevel() : Super()
	, light(nullptr)
	, rotateLight(nullptr)
	
	, plane(nullptr)
	, geosphere(nullptr)
	, teapot(nullptr)
	, cylinder(nullptr)
	, cone(nullptr)
	, icosahedron(nullptr)

	, spectator(nullptr)

	, sakura_miku(nullptr)
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

	LoadAssets(engine);

	light = SpawnActorPersistent<ADirectionalLight>();
	light->SetActorTransform(Transform(Vector3(0, 100, 0), Vector3::One, Quaternion::LookTo(Vector3(-1, -1, 1), Vector3(0, 1, 0))));
	light->LightComponent->LightColor = Color::White;
	light->LightComponent->Diffuse = 0.2f;
	light->LightComponent->Specular = 0;
	spotLight = SpawnActorPersistent<ASpotLight>();
	spotLight->LightComponent->LightColor = Color::Blue;
	spotLight->SetActorTransform(Transform(Vector3(1.0f, 4.0f, 0), Vector3::One, Quaternion::LookTo(Vector3(-1, -4, 0), Vector3(0, 0, 1))));

	// StreetLight
	{
		auto mesh = SpawnActorPersistent<AStaticMeshActor>();
		mesh->StaticMesh->SetStaticMesh(assetMgr->LoadStaticMesh(L"Engine/StaticMesh/Cylinder"));
		mesh->StaticMesh->Scale = Vector3(0.1f, 4.0f, 0.1f);
		mesh->StaticMesh->Location = Vector3(1.2f, 2.0f, 0);

		mesh = SpawnActorPersistent<AStaticMeshActor>();
		mesh->StaticMesh->SetStaticMesh(assetMgr->LoadStaticMesh(L"Engine/StaticMesh/GeoSphere"));
		mesh->StaticMesh->Scale = 0.2f;
		mesh->StaticMesh->Location = Vector3(1.2f, 4.0f, 0);
	}

	sakura_miku = SpawnActorPersistent<AStaticMeshActor>();
	sakura_miku->StaticMesh->SetStaticMesh(assetMgr->LoadStaticMesh(L"Content/Models/Sakura_Miku/Sakura_Miku.x"));
	sakura_miku->SetActorScale(0.1f);

	plane = SpawnActorPersistent<AStaticMeshActor>();
	plane->StaticMesh->SetStaticMesh(assetMgr->LoadStaticMesh(L"Engine/StaticMesh/Box"));
	plane->StaticMesh->Scale = Vector3(10.0f, 0.1f, 10.0f);
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
	GSpotLightComponent* pointLight = spectator->AddComponent<GSpotLightComponent>();
	pointLight->Mobility = EComponentMobility::Movable;
	pointLight->AttachToComponent(spectator->RootComponent);
	pointLight->LightColor = Color(0, 0.3f, 0);
	pointLight->RegisterComponentWithWorld(GetWorld());

	rotateLight = SpawnActorPersistent<ARotateLight>();
}

APawn* GStartupLevel::GetPersistentActor() const
{
	return spectator;
}

void GStartupLevel::LoadAssets(Engine* engine)
{
	ImportStaticMesh(engine, L"Content/Models/Sakura_Miku/Sakura_Miku.x");
}

void GStartupLevel::ImportStaticMesh(Engine* engine, TRefPtr<String> keyAndPath)
{
	AssetImporter* assimp = engine->GetAssetImporter();
	TRefPtr<StaticMesh> staticMesh = assimp->ImportStaticMesh(keyAndPath);

	AssetManager* assmgr = engine->GetAssetManager();
	assmgr->Import(keyAndPath, staticMesh);
}