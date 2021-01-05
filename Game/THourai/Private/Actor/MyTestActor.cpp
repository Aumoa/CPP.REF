// Copyright 2020 Aumoa.lib. All right reserved.

#include "Actor/MyTestActor.h"

#include "Components/StaticMeshComponent.h"

using namespace std;
using namespace std::chrono;

AMyTestActor::AMyTestActor() : Super()
	, staticMeshComponent(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	staticMeshComponent = AddComponent<StaticMeshComponent>();
	RootComponent = staticMeshComponent;

	StaticMesh* staticMesh = GAssetMgr.Load(L"Engine/StaticMesh/Triangle");
	staticMeshComponent->SetStaticMesh(staticMesh);
}

AMyTestActor::~AMyTestActor()
{

}

void AMyTestActor::Tick(duration<double> deltaTime)
{
	Super::Tick(deltaTime);
}