// Copyright 2020 Aumoa.lib. All right reserved.

#include "Pawn/MyCharacter.h"

#include "Components/SpringArmComponent.h"
#include "Components/CameraComponent.h"

AMyCharacter::AMyCharacter()
{
	auto* springArm = AddComponent<SpringArmComponent>();
	auto* camera = AddComponent<CameraComponent>();

	springArm->AttachToComponent(RootComponent);
	camera->AttachToSocket(springArm, SpringArmComponent::SocketName);
}

AMyCharacter::~AMyCharacter()
{

}