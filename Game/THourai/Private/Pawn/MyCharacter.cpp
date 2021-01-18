// Copyright 2020 Aumoa.lib. All right reserved.

#include "Pawn/MyCharacter.h"

#include "Components/SpringArmComponent.h"
#include "Components/CameraComponent.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	auto* springArm = AddComponent<SpringArmComponent>();
	auto* camera = AddComponent<CameraComponent>();

	springArm->AttachToComponent(RootComponent);
	camera->AttachToSocket(springArm, SpringArmComponent::SocketName);

	AddPrerequisiteObject(springArm);
}

AMyCharacter::~AMyCharacter()
{

}

void AMyCharacter::Tick(Seconds deltaTime)
{
	Super::Tick(deltaTime);
}