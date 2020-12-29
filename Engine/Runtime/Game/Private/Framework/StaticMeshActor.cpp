// Copyright 2020 Aumoa.lib. All right reserved.

#include "Framework/StaticMeshActor.h"

#include "Components/StaticMeshComponent.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game::Framework;
using namespace SC::Runtime::Game::Components;

AStaticMeshActor::AStaticMeshActor() : Super()
	, staticMeshComponent(nullptr)
{
	auto component = NewObject<StaticMeshComponent>();
	RootComponent = component.Get();
	staticMeshComponent = component.Get();
}

AStaticMeshActor::~AStaticMeshActor()
{

}