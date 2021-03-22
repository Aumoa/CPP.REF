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
	, hotaru(nullptr)
	, babara(nullptr)
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
	light->LightComponent->Ambient = 0.8f;
	light->LightComponent->Diffuse = 1.0f;
	light->LightComponent->Specular = 0.4f;
	rotateLight = SpawnActorPersistent<ARotateLight>();

	plane = SpawnActorPersistent<AStaticMeshActor>();
	plane->StaticMesh->SetStaticMesh(assetMgr->LoadStaticMesh(L"Content/Plane"));
	plane->StaticMesh->Scale = Vector3(10.0f, 0.1f, 10.0f);

	spectator = SpawnActorPersistent<ASpectatorPawn>();

	sakura_miku = SpawnActorPersistent<AStaticMeshActor>();
	sakura_miku->StaticMesh->SetStaticMesh(assetMgr->LoadStaticMesh(L"Content/Models/Sakura_Miku/Sakura_Miku.x"));
	sakura_miku->SetActorScale(0.1f);
	sakura_miku->SetActorLocation(Vector3(-1.0f, 0, 0));
	hotaru = SpawnActorPersistent<AStaticMeshActor>();
	hotaru->StaticMesh->SetStaticMesh(assetMgr->LoadStaticMesh(L"Content/Models/Hotaru/Hotaru.pmx"));
	hotaru->SetActorScale(0.1f);
	babara = SpawnActorPersistent<AStaticMeshActor>();
	babara->StaticMesh->SetStaticMesh(assetMgr->LoadStaticMesh(L"Content/Models/Babara/Babara.pmx"));
	babara->SetActorScale(0.1f);
	babara->SetActorLocation(Vector3(1.0f, 0, 0));
}

APawn* GStartupLevel::GetPersistentActor() const
{
	return spectator;
}

void GStartupLevel::LoadAssets(Engine* engine)
{
	AssetManager* assmgr = engine->GetAssetManager();

	StaticMeshGeometryData planeGeo;
	planeGeo.VertexBuffer =
	{
		Vertex(Vector3(-1, 0, 1), Vector3(0, 1, 0), Vector2(0, 0)),
		Vertex(Vector3(+1, 0, 1), Vector3(0, 1, 0), Vector2(1, 0)),
		Vertex(Vector3(+1, 0, -1), Vector3(0, 1, 0), Vector2(1, 1)),
		Vertex(Vector3(-1, 0, -1), Vector3(0, 1, 0), Vector2(0, 1)),
	};
	planeGeo.IndexBuffer =
	{
		0, 1, 3,
		1, 2, 3
	};
	planeGeo.Subsets.emplace_back(0, 4, 0, 6, assmgr->LoadMaterial(L"Engine/Materials/Default"));
	assmgr->Import(L"Content/Plane", NewObject<StaticMesh>(engine, planeGeo));

	ImportStaticMesh(engine, L"Content/Models/Sakura_Miku/Sakura_Miku.x");
	ImportStaticMesh(engine, L"Content/Models/Hotaru/Hotaru.pmx");
	ImportStaticMesh(engine, L"Content/Models/Babara/Babara.pmx");
}

void GStartupLevel::ImportStaticMesh(Engine* engine, TRefPtr<String> keyAndPath)
{
	AssetImporter* assimp = engine->GetAssetImporter();
	TRefPtr<StaticMesh> staticMesh = assimp->ImportStaticMesh(keyAndPath);

	AssetManager* assmgr = engine->GetAssetManager();
	assmgr->Import(keyAndPath, staticMesh);
}