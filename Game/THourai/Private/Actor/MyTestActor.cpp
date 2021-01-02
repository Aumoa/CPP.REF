// Copyright 2020 Aumoa.lib. All right reserved.

#include "Actor/MyTestActor.h"

#include "Components/StaticMeshComponent.h"

using namespace std;
using namespace std::chrono;

AMyTestActor::AMyTestActor()
{
	PrimaryActorTick.bCanEverTick = true;

	staticMeshComponent = NewObject<StaticMeshComponent>();
	RootComponent = staticMeshComponent.Get();

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