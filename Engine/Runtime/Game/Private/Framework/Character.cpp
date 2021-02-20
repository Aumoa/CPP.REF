// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Framework/Character.h"

#include "Components/SceneComponent.h"

ACharacter::ACharacter() : Super()
	, capsuleComponent(nullptr)
{
	capsuleComponent = AddComponent<SceneComponent>();
	RootComponent = capsuleComponent;
}

ACharacter::~ACharacter()
{

}